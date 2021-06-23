#include <interface99.h>

#include "common.h"

#include <assert.h>

typedef struct {
    char dummy;
} Foo2Impl;

declImpl(Foo2, Foo2Impl);

static void test_decl_impl(void) {
    assert(VTABLE(Foo2Impl, Foo2).a == a1_impl);
    assert(VTABLE(Foo2Impl, Foo2).b == b1_impl);
}

#define Foo2Impl_Foo2_a a1_impl
#define Foo2Impl_Foo2_b b1_impl
impl(Foo2, Foo2Impl);

int main(void) {
    test_decl_impl();
}
