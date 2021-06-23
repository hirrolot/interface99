#include <interface99.h>

#include "common.h"

#include <assert.h>

typedef struct {
    char dummy;
} Foo1Impl;

#define Foo1Impl_Foo1_a a1_impl
static impl(Foo1, Foo1Impl);

int main(void) {
    assert(VTABLE(Foo1Impl, Foo1).a == a1_impl);
}
