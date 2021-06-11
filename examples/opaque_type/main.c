// This example demonstrates how to implement interfaces for opaque types.
// Note: you can export interface implementations outside of a TU for non-opaque types in the same
// way.

#include "croak.h"
#include "frog.h"

static void test(Croak croakable) {
    croakable.vptr->croak(croakable.self);
}

int main(void) {
    Frog *john = Frog_new("John");
    Frog *paul = Frog_new("Paul");

    test(dyn(Frog, Croak, john));
    test(dyn(Frog, Croak, paul));

    Frog_free(john);
    Frog_free(paul);
}
