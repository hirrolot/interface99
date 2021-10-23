#include <interface99.h>

#include <assert.h>

#define METHODS(prefix, T)                                                                         \
    iMethod(int, prefix##_vcall, void *self)                                                       \
    iMethod(int, prefix##_vcall_args, void *self, int x, const char *y)                            \
                                                                                                   \
    iMethod(int, prefix##_vcall_obj, T self)                                                       \
    iMethod(int, prefix##_vcall_obj_args, T self, int x, const char *y)

#define TestSuper_IFACE METHODS(test_super, TestSuper)
#define Test_IFACE      METHODS(test, Test)
#define Test_EXTENDS    (TestSuper)

interface(TestSuper);
interface(Test);

static const int n = 7;
static const char *str = "abc";

typedef enum {
    TEST_VCALL,
    TEST_VCALL_ARGS,
    TEST_VCALL_OBJ,
    TEST_VCALL_OBJ_ARGS,
} RetVal;

typedef struct {
    char dummy;
} Impl;

static Impl obj = {0};

#define IMPL_METHODS(prefix, T)                                                                    \
    static int Impl_##prefix##_vcall(void *self) {                                                 \
        assert(&obj == self);                                                                      \
                                                                                                   \
        return TEST_VCALL;                                                                         \
    }                                                                                              \
                                                                                                   \
    static int Impl_##prefix##_vcall_args(void *self, int x, const char *y) {                      \
        assert(&obj == self);                                                                      \
        assert(n == x);                                                                            \
        assert(str == y);                                                                          \
                                                                                                   \
        return TEST_VCALL_ARGS;                                                                    \
    }                                                                                              \
                                                                                                   \
    static int Impl_##prefix##_vcall_obj(T test) {                                                 \
        assert(&obj == test.self);                                                                 \
        assert(&VTABLE(Impl, T) == test.vptr);                                                     \
                                                                                                   \
        return TEST_VCALL_OBJ;                                                                     \
    }                                                                                              \
                                                                                                   \
    static int Impl_##prefix##_vcall_obj_args(T test, int x, const char *y) {                      \
        assert(&obj == test.self);                                                                 \
        assert(&VTABLE(Impl, T) == test.vptr);                                                     \
        assert(n == x);                                                                            \
        assert(str == y);                                                                          \
                                                                                                   \
        return TEST_VCALL_OBJ_ARGS;                                                                \
    }

declImpl(TestSuper, Impl);
declImpl(Test, Impl);

// clang-format off
IMPL_METHODS(test_super, TestSuper)
IMPL_METHODS(test, Test)
// clang-format on

impl(TestSuper, Impl);
impl(Test, Impl);

int main(void) {
    Test test = DYN(Impl, Test, &obj);

    assert(TEST_VCALL == VCALL(test, test_vcall));
    assert(TEST_VCALL_ARGS == VCALL(test, test_vcall_args, n, str));
    assert(TEST_VCALL_OBJ == VCALL_OBJ(test, test_vcall_obj));
    assert(TEST_VCALL_OBJ_ARGS == VCALL_OBJ(test, test_vcall_obj_args, n, str));

    assert(TEST_VCALL == VCALL_SUPER(test, TestSuper, test_super_vcall));
    assert(TEST_VCALL_ARGS == VCALL_SUPER(test, TestSuper, test_super_vcall_args, n, str));
    assert(TEST_VCALL_OBJ == VCALL_SUPER_OBJ(test, TestSuper, test_super_vcall_obj));
    assert(
        TEST_VCALL_OBJ_ARGS == VCALL_SUPER_OBJ(test, TestSuper, test_super_vcall_obj_args, n, str));
}
