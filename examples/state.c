// This examples demonstrates the basic usage of Interface99.

#include <interface99.h>

#include <stdio.h>

// clang-format off
#define State_INTERFACE                 \
    method( int, get, void *self)       \
    method(void, set, void *self, int x)
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
    printf("x = %d\n", st.vptr->get(st.self));
    st.vptr->set(st.self, 5);
    printf("x = %d\n", st.vptr->get(st.self));
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
