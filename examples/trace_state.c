// This example demonstrates how to write interface decorators.

#include <interface99.h>

#include <stdio.h>

#define State_IFACE                                                                                \
    vfunc(int, get, const void *self)                                                              \
    vfunc(void, set, void *self, int data)

interface(State);

// Num implementation
// ============================================================

typedef struct {
    int data;
} Num;

int Num_get(const void *self) {
    return ((const Num *)self)->data;
}

void Num_set(void *self, int data) {
    ((Num *)self)->data = data;
}

impl(State, Num);

// TraceState implementation
// ============================================================

typedef struct {
    State st;
} TraceState;

int TraceState_get(const void *self) {
    const TraceState *this = (const TraceState *)self;
    int result = VCALL(this->st, get);
    printf("get() = %d\n", result);
    return result;
}

void TraceState_set(void *self, int data) {
    TraceState *this = (TraceState *)self;
    printf("set(%d)\n", data);
    VCALL(this->st, set, data);
}

impl(State, TraceState);

// Test
// ============================================================

void test(State st) {
    VCALL(st, set, 5);
    VCALL(st, set, 6);
    (void)VCALL(st, get);
}

/*
 * Output:
 * set(5)
 * set(6)
 * get() = 6
 */
int main(void) {
    State st = DYN(Num, State, &(Num){0}), trace_st = DYN(TraceState, State, &(TraceState){st});

    test(trace_st);

    return 0;
}
