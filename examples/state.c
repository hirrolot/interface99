#include <interface99.h>

#include <stdio.h>

#define State_INTERFACE                                                                            \
    iFn(int, get, void *self);                                                                     \
    iFn(void, set, void *self, int x);

interface(State);

typedef struct {
    int x;
} MyState;

int MyState_get(void *self) {
    return ((MyState *)self)->x;
}

void MyState_set(void *self, int x) {
    ((MyState *)self)->x = x;
}

impl(State, MyState);

void manipulate_state(State st) {
    printf("x = %d\n", st.vtable.get(st.self));
    st.vtable.set(st.self, 5);
    printf("x = %d\n", st.vtable.get(st.self));
}

/*
 * Output:
 * x = 0
 * x = 5
 */
int main(void) {
    MyState my_st = {.x = 0};
    State st = dyn(State, MyState, &my_st);
    manipulate_state(st);

    return 0;
}
