#ifndef TIMER_H
#define TIMER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "util.h"

// number chosen to be integer divisor of PIC frequency
#define TIMER_TPS 363

u64 timer_get();
void timer_init();

#ifdef __cplusplus
}
#endif
#endif
