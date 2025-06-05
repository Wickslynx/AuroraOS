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
  BLOCKED;
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

#endif
