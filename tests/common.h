#ifndef INTERFACE99_TESTS_COMMON_H
#define INTERFACE99_TESTS_COMMON_H

#include <interface99.h>

#include <stddef.h>

// Interfaces {

#define Marker_INTERFACE

#define Foo1_INTERFACE(FN, CTX) FN(CTX, const char *, a, int x, double *restrict y)

#define Foo2_INTERFACE(FN, CTX)                                                                    \
    FN(CTX, const char *, a, int x, double *restrict y)                                            \
    FN(CTX, float, b, long long x)

interface(Marker);
interface(Foo1);
interface(Foo2);
// } (Interfaces)

const char *a1_impl(int x, double *restrict y);
float b1_impl(long long x);
const char *a2_impl(int x, double *restrict y);
float b2_impl(long long x);

typedef const char *(*AFnType)(int x, double *restrict y);
typedef float (*BFnType)(long long x);

#endif // INTERFACE99_TESTS_COMMON_H
