// This examples demonstrates the basic usage of Interface99.

#include <interface99.h>

#include <stdio.h>

// clang-format off
#define State_IFACE                    \
    vfunc( int, get, void *self)       \
    vfunc(void, set, void *self, int x)
// clang-format on

interface(State);

typedef struct {
    int x;
} Num;

// clang-format off
int  Num_get(void *self) { return ((Num *)self)->x; }
void Num_set(void *self, int x) { ((Num *)self)->x = x; }
// clang-format on

impl(State, Num);

void test(State st) {
    printf("x = %d\n", VCALL(st, get));
    VCALL(st, set, 5);
    printf("x = %d\n", VCALL(st, get));
}

/*
 * Output:
 * x = 0
 * x = 5
 */
int main(void) {
    Num n = {0};
    State st = DYN(Num, State, &n);
    test(st);

    return 0;
}
