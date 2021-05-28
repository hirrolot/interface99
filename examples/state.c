#include <interface99.h>

#include <stdio.h>

#define State_INTERFACE                                                                            \
    iFn(int, get, void *self);                                                                     \
    iFn(void, set, void *self, int x);

interface(State);

typedef struct {
    int x;
} Num;

int Num_State_get(void *self) {
    return ((Num *)self)->x;
}

void Num_State_set(void *self, int x) {
    ((Num *)self)->x = x;
}

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
    Num my_st = {.x = 0};
    State st = dyn(State, Num, &my_st);
    test(st);

    return 0;
}
