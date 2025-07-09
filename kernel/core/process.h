#ifndef PROCESS_H
#define PROCESS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <error.h>
#include <util.h>
#include "../mem/vmm.h"

#include <log.h>
#include "domain.h"
#include "cpu.h"

typedef enum {
  RUNNING,
  READY,
  TERMINATED,
  BLOCKED,
  IDLE
} proc_state_t;

// pcb
typedef struct {
  int pid; // process id
  int ppid; // parent process id 
  int cpid[100]; // child process id 
  int nchild; // number of children processes
  int exitc; // exit code
  u32 usp; // user stack pointer
  u32 ksp; // kernel stack pointer.
  u32 entry;
  u32 stack;
  proc_state_t proc_state;
  Domain domain;
  page_dir_t page_dir;
  cpu_state_t cpu_t;
} Process;

Process* process_create(char* name, int ppid, Domain domain, u32 entry, u32 stack);
void process_destroy(Process* proc);
int load_program(Process* proc, void* code, int size) 

void scheduler(void);

extern Process* cproc;
extern Process* processes[256]; // TODO: Dynamic.. For now we shouldn't need more than this. (who tf has 256 proccesses open)
extern int cpid;

#ifdef __cplusplus
} 
#endif 

#endif
