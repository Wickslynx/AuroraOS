#ifndef PROCESS_H
#define PROCESS_H

#include "error.h"
#include "util.h"
#include "../mem/vmm.h"

#include "domain.h"
#include "cpu.h"

typedef enum {
  RUNNING,
  READY,
  TERMINATED,
  BLOCKED
} proc_state_t;

typedef struct {
  int pid;
  int ppid;
  int cpid[100];
  int exitc;
  u32 usp; // user stack pointer
  u32 ksp; // kernel stack pointer.
  proc_state_t proc_state;
  Domain domain;
  page_dir_t page_dir;
  cpu_state_t cpu_t;
} Process;

Process process_create(char* name, int ppid, Domain domain, u32 entry, u32 stack);
void process_destroy(Process* proc);

void process_set_state(Process *proc, proc_state_t state);
void process_switch();

extern Process* cproc;
extern Process* processes[256]; // TODO: Dynamic.. For now we shouldn't need more than this. (who tf has 256 open)

#endif
