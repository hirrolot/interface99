#include <interface99.h>

#include <stddef.h>

// Interfaces {

#define Marker_INTERFACE

#define Foo1_INTERFACE iFn(const char *, a, int x, double *restrict y);

#define Foo2_INTERFACE                                                                             \
    iFn(const char *, a, int x, double *restrict y);                                               \
    iFn(float, b, long long x);

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
