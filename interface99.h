/*
MIT License

Copyright (c) 2021 Hirrolot

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// The official repository: <https://github.com/Hirrolot/interface99>.

#ifndef INTERFACE99_H
#define INTERFACE99_H

#include <metalang99.h>

#if ML99_MAJOR != 1 && ML99_MINOR < 1
#error "Please, update Metalang99 to v1.1.0 or later"
#endif

#ifndef IFACE99_NO_ALIASES

#define interface   interface99
#define iFn         iFn99
#define impl        impl99
#define implPrimary implPrimary99
#define declImpl    declImpl99
#define dyn         dyn99
#define VTABLE      VTABLE99

#define Drop       Drop99
#define DropVTable Drop99VTable

#endif // IFACE99_NO_ALIASES

// Various public stuff {

// Metalang99-compliant macros {

#define IFACE99_interface(iface)         ML99_call(INTERFACE99_interface, iface)
#define IFACE99_impl(iface, implementor) ML99_call(INTERFACE99_impl, iface, implementor)
#define IFACE99_implPrimary(iface, implementor)                                                    \
    ML99_call(INTERFACE99_implPrimary, iface, implementor)

#define interface99(iface)                ML99_EVAL(IFACE99_interface_IMPL(iface))
#define impl99(iface, implementor)        ML99_EVAL(IFACE99_impl_IMPL(iface, implementor))
#define implPrimary99(iface, implementor) ML99_EVAL(IFACE99_implPrimary_IMPL(iface, implementor))
// }

#define IFACE99_MAJOR 0
#define IFACE99_MINOR 1
#define IFACE99_PATCH 0

// } (Various public stuff)

// Interface generation {

#define IFACE99_interface_IMPL(iface)                                                              \
    ML99_TERMS(                                                                                    \
        ML99_typedef(                                                                              \
            v(iface##VTable),                                                                      \
            ML99_struct(                                                                           \
                v(iface##VTable),                                                                  \
                ML99_uncomma(ML99_QUOTE(                                                           \
                    IFACE99_PRIV_genFnPtrForEach(v(IFACE99_PRIV_IFN_LIST(iface))),                 \
                    IFACE99_PRIV_genRequirementForEach(iface))))),                                 \
        v(typedef struct iface {                                                                   \
            void *self;                                                                            \
            const iface##VTable *vptr;                                                             \
        } iface))

#define IFACE99_PRIV_genFnPtrForEach(...)                                                          \
    ML99_variadicsForEach(ML99_compose(v(IFACE99_PRIV_genFnPtr), v(ML99_untuple)), __VA_ARGS__)

#define IFACE99_PRIV_genFnPtr_IMPL(ret_ty, name, ...) v(ret_ty (*name)(__VA_ARGS__);)

#define IFACE99_PRIV_genRequirementForEach(iface)                                                  \
    ML99_IF(                                                                                       \
        ML99_IS_TUPLE(iface##_REQUIRES),                                                           \
        ML99_variadicsForEach(v(IFACE99_PRIV_genRequirement), ML99_untuple(v(iface##_REQUIRES))),  \
        ML99_empty())

#define IFACE99_PRIV_genRequirement_IMPL(requirement) v(requirement##VTable requirement;)

// } (Interface generation)

// Interface implementation generation {

#define IFACE99_impl_IMPL(iface, implementor)                                                      \
    IFACE99_implAux(IFACE99_PRIV_genImplFnName, iface, implementor)
#define IFACE99_implPrimary_IMPL(iface, implementor)                                               \
    IFACE99_implAux(IFACE99_PRIV_genImplFnNamePrimary, iface, implementor)

#define IFACE99_implAux(gen_fn, iface, implementor)                                                \
    ML99_assign(                                                                                   \
        v(const iface##VTable VTABLE99(iface, implementor)),                                       \
        ML99_braced(ML99_uncomma(ML99_QUOTE(                                                       \
            IFACE99_PRIV_genImplFnNameForEach(                                                     \
                v(gen_fn),                                                                         \
                v(iface),                                                                          \
                v(implementor),                                                                    \
                v(IFACE99_PRIV_IFN_LIST(iface))),                                                  \
            IFACE99_PRIV_genRequirementsImplForEach(iface, implementor)))))

#define IFACE99_PRIV_genImplFnNameForEach(gen_fn, iface, implementor, ...)                         \
    ML99_variadicsForEach(                                                                         \
        ML99_compose(ML99_appl(gen_fn, iface, implementor), v(ML99_untuple)),                      \
        __VA_ARGS__)

#define IFACE99_PRIV_genImplFnName_IMPL(iface, implementor, _ret_ty, name, ...)                    \
    v(implementor##_##iface##_##name, )
#define IFACE99_PRIV_genImplFnNamePrimary_IMPL(_iface, implementor, _ret_ty, name, ...)            \
    v(implementor##_##name, )

#define IFACE99_PRIV_genRequirementsImplForEach(iface, implementor)                                \
    ML99_IF(                                                                                       \
        ML99_IS_TUPLE(iface##_REQUIRES),                                                           \
        ML99_variadicsForEach(                                                                     \
            ML99_appl(v(IFACE99_PRIV_genRequirementImpl), v(implementor)),                         \
            ML99_untuple(v(iface##_REQUIRES))),                                                    \
        ML99_empty())

#define IFACE99_PRIV_genRequirementImpl_IMPL(implementor, requirement)                             \
    v(VTABLE99(requirement, implementor), )
// } (Interface implementation generation)

#define declImpl99(iface, implementor) const ML99_CAT(iface, VTable) VTABLE99(iface, implementor)

#define IFACE99_PRIV_IFN_LIST(iface) ML99_VARIADICS_TAIL((iface##_INTERFACE))

#define iFn99(ret_ty, name, ...) ), (ret_ty, name, __VA_ARGS__) IFACE99_PRIV_EAT_INTERLEAVED_SEMICOLON ML99_LPAREN()

#define IFACE99_PRIV_EAT_INTERLEAVED_SEMICOLON ML99_EMPTY

#define dyn99(iface, implementor, ptr)                                                             \
    ((iface){.self = (void *)(ptr), .vptr = &VTABLE99(iface, implementor)})

#define VTABLE99(iface, implementor) ML99_CAT4(implementor, _, iface, _impl)

// Arity specifiers {
#define IFACE99_PRIV_genFnPtr_ARITY             1
#define IFACE99_PRIV_genImplFnName_ARITY        2
#define IFACE99_PRIV_genImplFnNamePrimary_ARITY 2
#define IFACE99_PRIV_genRequirement_ARITY       1
#define IFACE99_PRIV_genRequirementImpl_ARITY   2

// Public:
#define IFACE99_interface_ARITY   1
#define IFACE99_impl_ARITY        2
#define IFACE99_implPrimary_ARITY 2
// }

// Public interfaces {

#define Drop99_INTERFACE iFn99(void, drop, void *self);

interface99(Drop99);
// } (Public interfaces)

#endif // INTERFACE99_H
