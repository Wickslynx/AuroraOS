#ifndef MATH_H
#define MATH_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef double f64;

f64 fabs(f64 x);
f64 fmod(f64 x, f64 m);
f64 sin(f64 x);
f64 cos(f64 x);
f64 pow(f64 x, f64 y);

#ifdef MATH_IMPLEMENTATION

f64 fabs(f64 x) {
    return x < 0.0 ? -x : x;
}

f64 fmod(f64 x, f64 m) {
    f64 result;
    asm("1: fprem\n\t"
        "fnstsw %%ax\n\t"
        "sahf\n\t"
        "jp 1b"
        : "=t"(result) : "0"(x), "u"(m) : "ax", "cc");
    return result;
}

f64 sin(f64 x) {
    f64 result;
    asm("fsin" : "=t"(result) : "0"(x));
    return result;
}

f64 cos(f64 x) {
    return sin(x + M_PI / 2.0);
}

f64 pow(f64 x, f64 y) {
    f64 out;
    asm(
        "fyl2x;"
        "fld %%st;"
        "frndint;"
        "fsub %%st,%%st(1);"
        "fxch;"
        "fchs;"
        "f2xm1;"
        "fld1;"
        "faddp;"
        "fxch;"
        "fld1;"
        "fscale;"
        "fstp %%st(1);"
        "fmulp;" : "=t"(out) : "0"(x),"u"(y) : "st(1)" );
    return out;
}

#endif //MATH_IMPLEMENTATION

#endif // MATH_H
