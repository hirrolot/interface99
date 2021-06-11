// This example demonstrates how to implement interfaces for opaque types.
// Note: you can export interface implementations outside of a TU for non-opaque types in the same
// way.

#include "croak.h"
#include "frog.h"

static void test(Croak croakable) {
    croakable.vptr->croak(croakable.self);
}

/*
 * Output:
 * Paul: croak!
 * Steve: croak!
 */
int main(void) {
    Frog *paul = Frog_new("Paul");
    Frog *steve = Frog_new("Steve");

    test(dyn(Frog, Croak, paul));
    test(dyn(Frog, Croak, steve));

    Frog_free(paul);
    Frog_free(steve);
}
