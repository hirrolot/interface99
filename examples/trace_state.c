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

void manipulate_state(State st) {
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
    State st = dyn(State, MyState, &(MyState){.x = 0});
    State trace_st = dyn(State, TraceState, &(TraceState){.st = st});

    manipulate_state(trace_st);

    return 0;
}
