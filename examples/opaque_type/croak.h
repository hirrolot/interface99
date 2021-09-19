#ifndef OPAQUE_TYPE_CROAK_H
#define OPAQUE_TYPE_CROAK_H

#include <interface99.h>

#define Croak_INTERFACE(OP, ...) OP(__VA_ARGS__, void, croak, void *self)

interface(Croak);

#endif // OPAQUE_TYPE_CROAK_H
