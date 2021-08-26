#include <interface99.h>

#include "common.h"
#include "util.h"

#include <assert.h>

// Interfaces {

#define Foo1ExtendsMarker_INTERFACE Foo1_INTERFACE
#define Foo1ExtendsMarker_EXTENDS   (Marker)

#define Foo1ExtendsMany_INTERFACE Foo1_INTERFACE
#define Foo1ExtendsMany_EXTENDS   (Foo1, Foo2)

#define MarkerExtendsMarker_INTERFACE Marker_INTERFACE
#define MarkerExtendsMarker_EXTENDS   (Marker)

#define MarkerExtendsMany_INTERFACE Marker_INTERFACE
#define MarkerExtendsMany_EXTENDS   (Foo1, Foo2)

interface(Foo1ExtendsMarker);
interface(Foo1ExtendsMany);

interface(MarkerExtendsMarker);
interface(MarkerExtendsMany);
// } (Interfaces)

// Foo1ExtendsMarker {

typedef struct {
    char dummy;
} A;

impl(Marker, A);

#define A_Foo1ExtendsMarker_a a1_impl
impl(Foo1ExtendsMarker, A);
// } (Foo1ExtendsMarker)

// Foo1ExtendsMany {

typedef struct {
    char dummy;
} B;

#define B_Foo1_a a1_impl
impl(Foo1, B);

#define B_Foo2_a a1_impl
#define B_Foo2_b b1_impl
impl(Foo2, B);

#define B_Foo1ExtendsMany_a a1_impl
impl(Foo1ExtendsMany, B);
// } (Foo1ExtendsMany)

// MarkerExtendsMarker {

typedef struct {
    char dummy;
} C;

impl(Marker, C);
impl(MarkerExtendsMarker, C);
// } (MarkerExtendsMarker)

// MarkerExtendsMany {

typedef struct {
    char dummy;
} D;

#define D_Foo1_a a1_impl
impl(Foo1, D);

#define D_Foo2_a a1_impl
#define D_Foo2_b b1_impl
impl(Foo2, D);

impl(MarkerExtendsMany, D);
// } (MarkerExtendsMany)

int main(void) {
    // Ensure `interface`-generated data.
    {
        ENSURE_VTABLE_FIELD_TYPE(Foo1ExtendsMarkerVTable, a, AFnType);
        ENSURE_DYN_OBJ_TYPE(Foo1ExtendsMarker);

        ENSURE_VTABLE_FIELD_TYPE(Foo1ExtendsManyVTable, a, AFnType);
        ENSURE_DYN_OBJ_TYPE(Foo1ExtendsMany);

        ENSURE_DYN_OBJ_TYPE(MarkerExtendsMarker);

        ENSURE_DYN_OBJ_TYPE(MarkerExtendsMany);
    }

    // Ensure `impl`-generated data.
    {
        assert(VTABLE(A, Foo1ExtendsMarker).Marker == &VTABLE(A, Marker));

        assert(VTABLE(B, Foo1ExtendsMany).Foo1 == &VTABLE(B, Foo1));
        assert(VTABLE(B, Foo1ExtendsMany).Foo2 == &VTABLE(B, Foo2));

        assert(VTABLE(C, MarkerExtendsMarker).Marker == &VTABLE(C, Marker));

        assert(VTABLE(D, MarkerExtendsMany).Foo1 == &VTABLE(D, Foo1));
        assert(VTABLE(D, MarkerExtendsMany).Foo2 == &VTABLE(D, Foo2));
    }

    ENSURE_DYN_OBJ(A, Foo1ExtendsMarker);
    ENSURE_DYN_OBJ(B, Foo1ExtendsMany);
    ENSURE_DYN_OBJ(C, MarkerExtendsMarker);
    ENSURE_DYN_OBJ(D, MarkerExtendsMany);

    // Ensure that dynamic objects of superinterfaces still work.
    {
        ENSURE_DYN_OBJ(A, Marker);

        ENSURE_DYN_OBJ(B, Foo1);
        ENSURE_DYN_OBJ(B, Foo2);

        ENSURE_DYN_OBJ(C, Marker);

        ENSURE_DYN_OBJ(D, Foo1);
        ENSURE_DYN_OBJ(D, Foo2);
    }
}
