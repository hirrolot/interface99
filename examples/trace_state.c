// This example demonstrates how to write interface adaptors/decorators.

#include <interface99.h>

#include <stdio.h>

#define State_IFACE                                                                                \
    iMethod(int, get, void *self)                                                                  \
    iMethod(void, set, void *self, int x)

interface(State);

typedef struct {
    int x;
} Num;

int Num_get(void *self) { return ((Num *)self)->x; }
void Num_set(void *self, int x) { ((Num *)self)->x = x; }

impl(State, Num);

// A state adaptor that traces all methods on the inner state.
typedef struct {
    State st;
} TraceState;

int TraceState_get(void *self) {
    TraceState *this = (TraceState *)self;
    printf("get x\n");
    return VCALL(this->st, get);
}

void TraceState_set(void *self, int x) {
    TraceState *this = (TraceState *)self;
    printf("set x = %d\n", x);
    VCALL(this->st, set, x);
}

impl(State, TraceState);

void test(State st) {
    VCALL(st, set, 5);
    VCALL(st, set, 6);
    (void)VCALL(st, get);
}

/*
 * Output:
 * set x = 5
 * set x = 6
 * get x
 */
int main(void) {
    State st = DYN(Num, State, &(Num){0});
    State trace_st = DYN(TraceState, State, &(TraceState){st});

    test(trace_st);

    return 0;
}
