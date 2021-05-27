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

int main(void) {
    MyState state = {0};

    printf("Initial state = %d.\n", VTABLE(State, MyState).get(&state));
    VTABLE(State, MyState).set(&state, 5);
    printf("Updated state = %d.\n", VTABLE(State, MyState).get(&state));
}
