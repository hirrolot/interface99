// This example demonstrates how to write interface adaptors/decorators.

#include <interface99.h>

#include <stdio.h>

#define State_INTERFACE(OP, ...)                                                                   \
    OP(__VA_ARGS__, int, get, void *self)                                                          \
    OP(__VA_ARGS__, void, set, void *self, int x)

interface(State);

typedef struct {
    int x;
} Num;

int Num_get(void *self) { return ((Num *)self)->x; }

void Num_set(void *self, int x) { ((Num *)self)->x = x; }

impl(State, Num);

// A state adaptor that traces all operations on the inner state.
typedef struct {
    State st;
} TraceState;

int TraceState_get(void *self) {
    TraceState *this = (TraceState *)self;

    printf("get x\n");
    return this->st.vptr->get(this->st.self);
}

void TraceState_set(void *self, int x) {
    TraceState *this = (TraceState *)self;

    printf("set x = %d\n", x);
    this->st.vptr->set(this->st.self, x);
}

impl(State, TraceState);

void test(State st) {
    st.vptr->set(st.self, 5);
    st.vptr->set(st.self, 6);
    (void)st.vptr->get(st.self);
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
