#ifndef INTERFACE99_TESTS_UTIL_H
#define INTERFACE99_TESTS_UTIL_H

#define ENSURE_EXPR_TYPE(expr, type)                                                               \
    do {                                                                                           \
        type x = (expr);                                                                           \
        (void)x;                                                                                   \
    } while (0)
#define ENSURE_FIELD_TYPE(struct, field, type) ENSURE_EXPR_TYPE(((struct){0}).field, type)

#define ENSURE_DYN_OBJ_TYPE(iface)                                                                 \
    ENSURE_FIELD_TYPE(iface, self, void *);                                                        \
    ENSURE_FIELD_TYPE(iface, vptr, const iface##VTable *);

#define ENSURE_DYN_OBJ(implementor, iface)                                                         \
    do {                                                                                           \
        implementor x = {0};                                                                       \
        iface x_dyn = dyn(implementor, iface, &x);                                                 \
        assert(x_dyn.self = &x);                                                                   \
        assert(x_dyn.vptr = &VTABLE(implementor, iface));                                          \
    } while (0)

#endif // INTERFACE99_TESTS_UTIL_H
