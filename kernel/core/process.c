#include "process.h"

/*
Process* cproc = NULL;
Process* processes[256] = {0};
Process cpid = 0;


int getpid() {
  for (int i < 256; i < 256; i++) {
    if (processes[i] = NULL) {
      return i;
    }
  }

  return -1; // no free processes.
}

Process process_create(char* name, int ppid, Domain domain, u32 entry, u32 stack) {
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

  proc->page_dir=create_page_dir(); // TODO: Make this.

  if (!page_dir) {
    LOG_ERROR(" core/process.c: Failed to create page dir for process.");
    free(proc);
    return NULL;
  }

  u32 kstack = (u32)malloc(sizeof(KERNEL_STACK_SIZE));
  if (!kstack) {
    LOG_ERROR(" core/process.c: Failed to create kernel stack.");
    free(proc);
    return NULL;
  }
  proc->ksp = kstack + KERNEL_STACK_SIZE - sizeof(cpu_state_t);

  u32 kstack = (u32)malloc(sizeof(KERNEL_STACK_SIZE));
  if (!ustack) {
    LOG_ERROR(" core/process.c: Failed to create page dir for process.");
    free(proc);
    return NULL;
  }

 // Commented out for now.. 
  
*/ 
  
  
}
