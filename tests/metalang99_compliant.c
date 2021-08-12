#include <interface99.h>

#include "util.h"

#include <metalang99.h>

#include <assert.h>
#include <stddef.h>

ML99_ASSERT_UNEVAL(IFACE99_interface_ARITY == 1);
ML99_ASSERT_UNEVAL(IFACE99_impl_ARITY == 2);
ML99_ASSERT_UNEVAL(IFACE99_implPrimary_ARITY == 2);
ML99_ASSERT_UNEVAL(IFACE99_externImpl_ARITY == 2);
ML99_ASSERT_UNEVAL(IFACE99_externImplPrimary_ARITY == 2);

// Interfaces {
#define Foo1_INTERFACE iFn(const char *, a, int x, double *restrict y);

ML99_EVAL(IFACE99_interface(v(Foo1)));
// } (Interfaces)

// Implementations {
static const char *a_impl(int x, double *restrict y) {
    (void)x;
    (void)y;
    return (const char *)NULL;
}

typedef struct {
    char dummy;
} A;

#define A_Foo1_a a_impl
ML99_EVAL(IFACE99_impl(v(Foo1), v(A)));

typedef struct {
    char dummy;
} B;

#define B_a a_impl
ML99_EVAL(IFACE99_implPrimary(v(Foo1), v(B)));

typedef struct {
    char dummy;
} C;

#define C_Foo1_a a_impl
ML99_EVAL(IFACE99_externImpl(v(Foo1), v(C)));

typedef struct {
    char dummy;
} D;

#define D_a a_impl
ML99_EVAL(IFACE99_externImplPrimary(v(Foo1), v(D)));
// } (Implementations)

int main(void) {
    // Ensure `impl`-generated data.
    {
        assert(VTABLE(A, Foo1).a == a_impl);
        assert(VTABLE(B, Foo1).a == a_impl);

        assert(VTABLE(C, Foo1).a == a_impl);
        assert(VTABLE(D, Foo1).a == a_impl);
    }

    ENSURE_DYN_OBJ(A, Foo1);
    ENSURE_DYN_OBJ(B, Foo1);

    ENSURE_DYN_OBJ(C, Foo1);
    ENSURE_DYN_OBJ(D, Foo1);
}
