#ifndef INTERFACE99_TESTS_UTIL_H
#define INTERFACE99_TESTS_UTIL_H

#define ENSURE_EXPR_TYPE(expr, type)                                                               \
    do {                                                                                           \
        type x = (expr);                                                                           \
        (void)x;                                                                                   \
    } while (0)

#define ENSURE_VTABLE_FIELD_TYPE(vtable_type, field, type)                                         \
    do {                                                                                           \
        ENSURE_FIELD_TYPE(vtable_type, field, type);                                               \
        ENSURE_FIELD_TYPE(struct vtable_type, field, type);                                        \
    } while (0)
#define ENSURE_FIELD_TYPE(outer_type, field, type) ENSURE_EXPR_TYPE(((outer_type){0}).field, type)

#define ENSURE_DYN_OBJ_TYPE(iface)                                                                 \
    do {                                                                                           \
        ENSURE_FIELD_TYPE(iface, self, void *);                                                    \
        ENSURE_FIELD_TYPE(iface, vptr, const iface##VTable *);                                     \
                                                                                                   \
        ENSURE_FIELD_TYPE(struct iface, self, void *);                                             \
        ENSURE_FIELD_TYPE(struct iface, vptr, const iface##VTable *);                              \
    } while (0)

#define ENSURE_DYN_OBJ(implementer, iface)                                                         \
    do {                                                                                           \
        implementer x = {0};                                                                       \
        iface x_dyn = DYN(implementer, iface, &x);                                                 \
        assert(x_dyn.self = &x);                                                                   \
        assert(x_dyn.vptr = &VTABLE(implementer, iface));                                          \
    } while (0)

#endif // INTERFACE99_TESTS_UTIL_H
