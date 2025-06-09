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

  proc->page_dir = create_page(); // TODO: Make this.

  if (!proc->page_dir) { 
    LOG_ERROR(" core/process.c: Failed to create page dir for process.");
    free(proc);
    return NULL;
  }

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
  cpu->cs  = 0x1B;               // user mode (ring 3)
  cpu->eflags = 0x202;          
  cpu->esp = proc->usp;        
  cpu->ss  = 0x23;               // user data (ring 3)

  // set the table slot.
  processes[pid] = proc;

  LOG_INFO(" core/process.c: Created process!");
  return proc;
}


void cswitch(cpu_state_t* new_state) {
  asm (
      "movl %0, %%esp \n"  
      "popa \n"            
      "add $8, %%esp \n"   
      "iret \n"            
      :
      : "r"(new_state)
      : "memory"
  );
}

void switch_proc(Process* next_proc) {
    if (!next_proc) return;

    cproc = next_proc; 
    cswitch((cpu_state_t*)(next_proc->ksp));
}

void schedule() {
  int npid = (cproc->pid + 1) % 256;

  for (int i = 0; i < 256; i++) { 
    int pid = (npid + i) % 256;
    if (processes[pid] && processes[pid]->proc_state == READY) {
      if (processes[pid] != cproc) {
        switch_proc(processes[pid]);
      }
      return;
    }
  }
  
  return; // stay on current process
}
