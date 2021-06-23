#include <interface99.h>

#include "common.h"

#include <assert.h>

typedef struct {
    char dummy;
} Foo2Impl;

#define Foo2Impl_a a1_impl
#define Foo2Impl_b b1_impl
implPrimary(Foo2, Foo2Impl);

int main(void) {
    assert(VTABLE(Foo2Impl, Foo2).a == a1_impl);
    assert(VTABLE(Foo2Impl, Foo2).b == b1_impl);
}
