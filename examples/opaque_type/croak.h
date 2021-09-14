#ifndef OPAQUE_TYPE_CROAK_H
#define OPAQUE_TYPE_CROAK_H

#include <interface99.h>

#define Croak_INTERFACE(fn, ctx) fn(ctx, void, croak, void *self)

interface(Croak);

#endif // OPAQUE_TYPE_CROAK_H
