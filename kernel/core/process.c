#include "process.h"

#define KERNEL_STACK_SIZE 8192

Process* cproc = NULL;
Process* processes[256] = {0};
int cpid = 0;

int getpid() {
  for (int i = 0; i < 256; i++) { 
    if (processes[i] == NULL) {   
      return i;
    }
  }
  return -1; // no free processes.
}

Process* process_create(char* name, int ppid, Domain domain, u32 entry, u32 stack) {
  int pid = getpid();

  if (pid == -1) {
    LOG_WARNING(" core/process.c: Too many processes currently running...");
    return NULL;
  }

  Process* proc = (Process*)malloc(sizeof(Process));
  if (!proc) {
    LOG_ERROR(" core/process.c: Failed to allocate process struct...");
    return NULL;
  }

  proc->pid = pid;
  proc->ppid = ppid;
  proc->nchild = 0;
  proc->exitc = 0;
  proc->proc_state = READY;
  proc->domain = domain;

  // clear children array (100)
  memset(proc->cpid, 0, sizeof(proc->cpid));

  
  create_page(&proc->page_dir);
  
  u32 kstack = (u32)malloc(KERNEL_STACK_SIZE); // kernel stack
  if (!kstack) {
    LOG_ERROR(" core/process.c: Failed to create kernel stack.");
    free(proc);
    return NULL;
  }
  proc->ksp = kstack + KERNEL_STACK_SIZE - sizeof(cpu_state_t); // top of kernel stack

  u32 ustack = (u32)malloc(KERNEL_STACK_SIZE); // user stack
  if (!ustack) {
    LOG_ERROR(" core/process.c: Failed to create user stack.");
    free((void*)kstack);
    free(proc);
    return NULL;
  }
  proc->usp = ustack + KERNEL_STACK_SIZE - sizeof(cpu_state_t); // top of user stack

  proc->entry = entry; // entry
  proc->stack = stack; // user stack

  cpu_state_t* cpu = (cpu_state_t*)(proc->ksp);
  memset(cpu, 0, sizeof(cpu_state_t)); // Clear CPU state

  cpu->eip = entry;
  cpu->cs = 0x1B;        // user code (ring 3)
  cpu->eflags = 0x202;   // sti
  cpu->esp = proc->usp;  
  cpu->ss = 0x23;        // user data (ring 3)
  cpu->ds = 0x23;        // data
  cpu->es = 0x23;
  cpu->fs = 0x23;
  cpu->gs = 0x23;


  // set the table slot.
  processes[pid] = proc;

  LOG_INFO(" core/process.c: Created process!");
  return proc;
}

void process_destroy(Process* proc) {
    // Remove from process table
    processes[proc->pid] = NULL;

    // Mark as terminated
    proc->proc_state = TERMINATED;

    // Free kernel stack
    free((void*)(proc->ksp - (KERNEL_STACK_SIZE - sizeof(cpu_state_t))));

    // Free user stack
    free((void*)(proc->usp - (KERNEL_STACK_SIZE - sizeof(cpu_state_t))));

    // Free the process struct itself
    free(proc);

    // NOTE: No file descriptor cleanup, and parent/child handling is manual
      
}

int load_program(Process* proc, void* code, int size) {
    if (!proc || !code || size == 0) {
        return -1;
    }
    
    memcpy((void*)proc->entry, code, size);
    return 0;
}

void cswitch(cpu_state_t* old_state, cpu_state_t* new_state) {
    asm   (
        // save
        "pushf \n"              // flags
        "pusha \n"              // regs
        "movl %%esp, %0 \n"     // sp
        
        // switch
        "movl %1, %%esp \n"     // new sp
        "popa \n"               // regs
        "popf \n"               // flags
        "ret \n"                // return
        
        : "=m" (*old_state)     // out: old state
        : "m" (*new_state)      // in: new state
        : "memory"
    );
}

void switch_proc(Process* next_proc) {
    if (!next_proc) return;
    
    Process* old_proc = cproc;
    cproc = next_proc;
    
    if (old_proc) {
        cswitch((cpu_state_t*)(old_proc->ksp), (cpu_state_t*)(next_proc->ksp));
    } else {
        // first proc ever.
        asm   (
            "movl %0, %%esp \n"  
            "popa \n"            
            "add $8, %%esp \n"   
            "iret \n"            
            :
            : "r"((cpu_state_t*)(next_proc->ksp))
            : "memory"
        );
    }
}

void scheduler(void) {
    Process* next_proc = NULL;
  
    int start_pid = cproc ? (cproc->pid + 1) % 256 : 0; // no current process.
    
    // next proc
    for (int i = 0; i < 256; i++) { 
        int pid = (start_pid + i) % 256;
        if (processes[pid] && processes[pid]->proc_state == READY) {
            next_proc = processes[pid];
            break;
        }
    }
    
    // if theres no ready proc.
    if (!next_proc) {
        if (cproc && (cproc->proc_state == RUNNING || cproc->proc_state == READY)) {
            // c proc can just run untill we have another one.
            cproc->proc_state = RUNNING;
            return;
        } else {
            // set c proc to idle 
            cproc->proc_state = IDLE;
            return;
        }
    }
    
    
    if (cproc && cproc->proc_state == RUNNING) {
        cproc->proc_state = READY;  
    }
    
    // switch
    next_proc->proc_state = RUNNING;
    switch_proc(next_proc);
}
