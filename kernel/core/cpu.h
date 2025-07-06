#ifndef CPU_H
#define CPU_H

#include <util.h>


typedef struct cpu_state {
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax, es; 
    u32 int_no, err_code;                       
    u32 eip, cs, gs, ds, fs, eflags, useresp, ss;           
} cpu_state_t;

#endif // CPU_H
