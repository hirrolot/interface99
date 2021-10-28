#include "common.h"

const char *foo1_impl(int x, double *restrict y) {
    (void)x;
    (void)y;
    return (const char *)NULL;
}

float bar1_impl(long long x) {
    (void)x;
    return 12.141f;
}

const char *foo2_impl(int x, double *restrict y) {
    return foo1_impl(x, y);
}

float bar2_impl(long long x) {
    return bar1_impl(x);
}
