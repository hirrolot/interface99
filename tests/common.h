#ifndef INTERFACE99_TESTS_COMMON_H
#define INTERFACE99_TESTS_COMMON_H

#include <interface99.h>

#include <stddef.h>

// Interfaces {

#define Marker_INTERFACE

#define Foo_INTERFACE method(const char *, foo, int x, double *restrict y)

#define Bar_INTERFACE                                                                              \
    method(const char *, foo, int x, double *restrict y)                                           \
    method(float, bar, long long x)

interface(Marker);
interface(Foo);
interface(Bar);
// } (Interfaces)

const char *foo1_impl(int x, double *restrict y);
float bar1_impl(long long x);

const char *foo2_impl(int x, double *restrict y);
float bar2_impl(long long x);

typedef const char *(*FooOpType)(int x, double *restrict y);
typedef float (*BarOpType)(long long x);

#endif // INTERFACE99_TESTS_COMMON_H
