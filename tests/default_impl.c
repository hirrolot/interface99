#include <interface99.h>

#include <assert.h>

#define TestDefault_INTERFACE(OP, ...) OP(__VA_ARGS__, void, default_op, void *self, int x)
#define TestDefault_default_op_DEFAULT ()

interface(TestDefault);

static void TestDefault_default_op(void *self, int x) {
    (void)self;
    (void)x;
}

static void custom_impl(void *self, int x) {
    (void)self;
    (void)x;
}

typedef struct {
    char dummy;
} A;

impl(TestDefault, A);

typedef struct {
    char dummy;
} B;

#define B_default_op_CUSTOM ()
#define B_default_op        custom_impl
impl(TestDefault, B);

#define TestNoOpCustom_INTERFACE(OP, ...) OP(__VA_ARGS__, void, custom_op, void *self, int x)

interface(TestNoOpCustom);

typedef struct {
    char dummy;
} C;

// This `*_CUSTOM` attribute must be no-op.
#define C_custom_op_CUSTOM ()
#define C_custom_op        custom_impl
impl(TestNoOpCustom, C);

int main(void) {
    assert(VTABLE(A, TestDefault).default_op == TestDefault_default_op);
    assert(VTABLE(B, TestDefault).default_op == custom_impl);
    assert(VTABLE(C, TestNoOpCustom).custom_op == custom_impl);
}
