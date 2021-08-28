#include <interface99.h>

#include "common.h"
#include "util.h"

#include <assert.h>
#include <stddef.h>

// Implementations {

typedef struct {
    char dummy;
} MarkerImpl;

impl(Marker, MarkerImpl);

typedef struct {
    char dummy;
} Foo1Impl;

#define Foo1Impl_Foo1_a a1_impl
impl(Foo1, Foo1Impl);

typedef struct {
    char dummy;
} Foo2Impl1;

#define Foo2Impl1_Foo2_a a1_impl
#define Foo2Impl1_Foo2_b b1_impl
impl(Foo2, Foo2Impl1);

// Ensure that an interface can be implemented by many types.
typedef struct {
    char dummy;
} Foo2Impl2;

#define Foo2Impl2_Foo2_a a2_impl
#define Foo2Impl2_Foo2_b b2_impl
impl(Foo2, Foo2Impl2);

// Ensure that a type can implement multiple interfaces.
typedef struct {
    char dummy;
} Foo1Foo2Impl;

#define Foo1Foo2Impl_Foo1_a a1_impl
impl(Foo1, Foo1Foo2Impl);

#define Foo1Foo2Impl_Foo2_a a1_impl
#define Foo1Foo2Impl_Foo2_b b1_impl
impl(Foo2, Foo1Foo2Impl);
// } (Implementations)

int main(void) {
    // Ensure `interface`-generated data.
    {
        ENSURE_VTABLE_FIELD_TYPE(MarkerVTable, dummy, char);
        ENSURE_DYN_OBJ_TYPE(Marker);

        ENSURE_VTABLE_FIELD_TYPE(Foo1VTable, a, AFnType);
        ENSURE_DYN_OBJ_TYPE(Foo1);

        ENSURE_VTABLE_FIELD_TYPE(Foo2VTable, a, AFnType);
        ENSURE_VTABLE_FIELD_TYPE(Foo2VTable, b, BFnType);
        ENSURE_DYN_OBJ_TYPE(Foo2);
    }

    // Ensure `impl`-generated data.
    {
        assert(VTABLE(MarkerImpl, Marker).dummy == '\0');

        assert(VTABLE(Foo1Impl, Foo1).a == a1_impl);

        assert(VTABLE(Foo2Impl1, Foo2).a == a1_impl);
        assert(VTABLE(Foo2Impl1, Foo2).b == b1_impl);

        assert(VTABLE(Foo2Impl2, Foo2).a == a2_impl);
        assert(VTABLE(Foo2Impl2, Foo2).b == b2_impl);

        assert(VTABLE(Foo1Foo2Impl, Foo1).a == a1_impl);
        assert(VTABLE(Foo1Foo2Impl, Foo2).a == a1_impl);
        assert(VTABLE(Foo1Foo2Impl, Foo2).b == b1_impl);
    }

    ENSURE_DYN_OBJ(MarkerImpl, Marker);
    ENSURE_DYN_OBJ(Foo1Impl, Foo1);
    ENSURE_DYN_OBJ(Foo2Impl1, Foo2);
    ENSURE_DYN_OBJ(Foo2Impl2, Foo2);
    ENSURE_DYN_OBJ(Foo1Foo2Impl, Foo1);
    ENSURE_DYN_OBJ(Foo1Foo2Impl, Foo2);

    // IFACE99_IS_NON_NULL
    {
        Foo1 foo1 = DYN(Foo1Impl, Foo1, &(Foo1Impl){0});
        assert(IFACE99_IS_NON_NULL(foo1));

        foo1.self = NULL;
        assert(!IFACE99_IS_NON_NULL(foo1));

        foo1.vptr = NULL;
        assert(!IFACE99_IS_NON_NULL(foo1));
    }
}
