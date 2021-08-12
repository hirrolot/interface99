#include "frog.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct Frog {
    const char *name;
    int ncroaks;
};

Frog *Frog_new(const char *name) {
    Frog *self = malloc(sizeof *self);
    assert(self);

    self->name = name;
    self->ncroaks = 0;

    return self;
}

void Frog_free(Frog *self) {
    assert(self);
    free(self);
}

static void Frog_Croak_croak(void *self) {
    Frog *this = (Frog *)self;
    printf("%s: croak!\n", this->name);
    this->ncroaks++;
}

externImpl(Croak, Frog);
