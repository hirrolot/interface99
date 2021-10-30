#ifndef OPAQUE_TYPE_FROG_H
#define OPAQUE_TYPE_FROG_H

#include <interface99.h>

#include "croak.h"

typedef struct Frog Frog;

Frog *Frog_new(const char *name);
void Frog_free(Frog *self);

declImplExtern(Croak, Frog);

#endif // OPAQUE_TYPE_FROG_H
