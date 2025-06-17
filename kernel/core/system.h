#ifndef SYSTEM_H
#define SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "util.h"

typedef struct { // Not finished, this should hold all system level stuff.
        bool quit;
} system;

#define _assert_0() __error_illegal_macro__
#define _assert_1(_e) do { if (!(_e)) panic(NULL); } while (0)
#define _assert_2(_e, _m) do { if (!(_e)) panic((_m)); } while (0)

#define _assert(x, _e, _m, _f, ...) _f

#define assert(...) _assert(,##__VA_ARGS__,\
        _assert_2(__VA_ARGS__),\
        _assert_1(__VA_ARGS__),\
        _assert_0(__VA_ARGS__))

u32 rand();
void seed(u32 s);

#ifdef __cplusplus
}
#endif 

#endif
