#include "common.h"

const char *a1_impl(int x, double *restrict y) {
    (void)x;
    (void)y;
    return (const char *)NULL;
}

float b1_impl(long long x) {
    (void)x;
    return 12.141f;
}

const char *a2_impl(int x, double *restrict y) {
    return a1_impl(x, y);
}

float b2_impl(long long x) {
    return b1_impl(x);
}
