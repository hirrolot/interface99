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

#if (ML99_MAJOR == 1 && ML99_MINOR < 2) || (ML99_MAJOR > 1)
#error "Please, update Metalang99 to v1.2.0 or later"
#endif

#ifndef IFACE99_NO_ALIASES

#define interface   interface99
#define iFn         iFn99
#define impl        impl99
#define implPrimary implPrimary99
#define declImpl    declImpl99
#define dyn         dyn99
#define VTABLE      VTABLE99

#endif // IFACE99_NO_ALIASES

// Various public stuff {

// Metalang99-compliant macros {

#define IFACE99_interface(iface)                ML99_call(IFACE99_interface, iface)
#define IFACE99_impl(iface, implementor)        ML99_call(IFACE99_impl, iface, implementor)
#define IFACE99_implPrimary(iface, implementor) ML99_call(IFACE99_implPrimary, iface, implementor)

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
        v(typedef struct iface##VTable iface##VTable;),                                            \
        v(typedef struct iface iface;),                                                            \
        ML99_semicoloned(ML99_struct(v(iface##VTable), IFACE99_PRIV_genVTableFields(iface))),      \
        v(struct iface {                                                                           \
            void *self;                                                                            \
            const iface##VTable *vptr;                                                             \
        }))

/*
 * // Only if <iface> is a marker interface without superinterfaces:
 * char dummy;
 *
 * <fn-ret-ty>0 (*<fn-name>0)(<fn-params>0);
 * ...
 * <fn-ret-ty>N (*<fn-name>N)(<fn-params>N);
 *
 * const <requirement>0VTable *<requirement>;
 * ...
 * const <requirement>NVTable *<requirement>;
 */
#define IFACE99_PRIV_genVTableFields(iface)                                                        \
    ML99_uncomma(ML99_QUOTE(                                                                       \
        IFACE99_PRIV_genDummy(iface),                                                              \
        ML99_IF(                                                                                   \
            IFACE99_PRIV_IS_MARKER_IFACE(iface),                                                   \
            ML99_empty(),                                                                          \
            ML99_callUneval(IFACE99_PRIV_genFnPtrForEach, iface)),                                 \
        ML99_IF(                                                                                   \
            IFACE99_PRIV_IS_SUB_IFACE(iface),                                                      \
            IFACE99_PRIV_genRequirementForEach(iface),                                             \
            ML99_empty())))

/*
 * <fn-ret-ty>0 (*<fn-name>0)(<fn-params>0);
 * ...
 * <fn-ret-ty>N (*<fn-name>N)(<fn-params>N);
 */
#define IFACE99_PRIV_genFnPtrForEach_IMPL(iface)                                                   \
    ML99_variadicsForEach(                                                                         \
        ML99_compose(v(IFACE99_PRIV_genFnPtr), v(ML99_untuple)),                                   \
        v(IFACE99_PRIV_IFN_LIST(iface)))

#define IFACE99_PRIV_genFnPtr_IMPL(ret_ty, name, ...) v(ret_ty (*name)(__VA_ARGS__);)

/*
 * const <requirement>0VTable *<requirement>;
 * ...
 * const <requirement>NVTable *<requirement>;
 */
#define IFACE99_PRIV_genRequirementForEach(iface)                                                  \
    ML99_variadicsForEach(v(IFACE99_PRIV_genRequirement), ML99_untuple(v(iface##_EXTENDS)))

#define IFACE99_PRIV_genRequirement_IMPL(requirement) v(const requirement##VTable *requirement;)

// } (Interface generation)

// Interface implementation generation {

#define IFACE99_impl_IMPL(iface, implementor)                                                      \
    IFACE99_PRIV_implAux(IFACE99_PRIV_GEN_IMPL_FN_NAME, iface, implementor)
#define IFACE99_implPrimary_IMPL(iface, implementor)                                               \
    IFACE99_PRIV_implAux(IFACE99_PRIV_GEN_IMPL_FN_NAME_PRIMARY, iface, implementor)

#define IFACE99_PRIV_implAux(gen_fn_name, iface, implementor)                                      \
    ML99_assign(                                                                                   \
        v(const iface##VTable VTABLE99(implementor, iface)),                                       \
        ML99_braced(IFACE99_PRIV_genImplInitList(gen_fn_name, iface, implementor)))

/*
 * // Only if <iface> is a marker interface without superinterfaces:
 * .dummy = '\0',
 *
 * <fn-name>0 = <implementor>_<iface>_<fn-name>0 (or <implementor>_<fn-name>0),
 * ...
 * <fn-name>N = <implementor>_<iface>_<fn-name>N (or <implementor>_<fn-name>N),
 *
 * <requirement>0 = &VTABLE(<implementor, <requirement>0),
 * ...
 * <requirement>N = &VTABLE(<implementor, <requirement>N),
 */
#define IFACE99_PRIV_genImplInitList(gen_fn_name, iface, implementor)                              \
    ML99_uncomma(ML99_QUOTE(                                                                       \
        IFACE99_PRIV_genDummyInit(iface),                                                          \
        ML99_IF(                                                                                   \
            IFACE99_PRIV_IS_MARKER_IFACE(iface),                                                   \
            ML99_empty(),                                                                          \
            ML99_callUneval(IFACE99_PRIV_genImplFnNameForEach, gen_fn_name, iface, implementor)),  \
        ML99_IF(                                                                                   \
            IFACE99_PRIV_IS_SUB_IFACE(iface),                                                      \
            IFACE99_PRIV_genRequirementsImplForEach(iface, implementor),                           \
            ML99_empty())))

/*
 * <fn-name>0 = <implementor>_<iface>_<fn-name>0 (or <implementor>_<fn-name>0),
 * ...
 * <fn-name>N = <implementor>_<iface>_<fn-name>N (or <implementor>_<fn-name>N),
 */
#define IFACE99_PRIV_genImplFnNameForEach_IMPL(gen_fn_name, iface, implementor)                    \
    ML99_variadicsForEach(                                                                         \
        ML99_compose(                                                                              \
            ML99_appl(v(IFACE99_PRIV_genImplFnName), v(gen_fn_name, iface, implementor)),          \
            v(ML99_untuple)),                                                                      \
        v(IFACE99_PRIV_IFN_LIST(iface)))

#define IFACE99_PRIV_genImplFnName_IMPL(gen_fn_name, iface, implementor, _ret_ty, name, ...)       \
    v(.name = gen_fn_name(iface, implementor, name), )

#define IFACE99_PRIV_GEN_IMPL_FN_NAME(iface, implementor, name)          implementor##_##iface##_##name
#define IFACE99_PRIV_GEN_IMPL_FN_NAME_PRIMARY(_iface, implementor, name) implementor##_##name

/*
 * <requirement>0 = &VTABLE(<implementor, <requirement>0),
 * ...
 * <requirement>N = &VTABLE(<implementor, <requirement>N),
 */
#define IFACE99_PRIV_genRequirementsImplForEach(iface, implementor)                                \
    ML99_variadicsForEach(                                                                         \
        ML99_appl(v(IFACE99_PRIV_genRequirementImpl), v(implementor)),                             \
        ML99_untuple(v(iface##_EXTENDS)))

#define IFACE99_PRIV_genRequirementImpl_IMPL(implementor, requirement)                             \
    v(.requirement = &VTABLE99(implementor, requirement), )
// } (Interface implementation generation)

#define declImpl99(iface, implementor) const ML99_CAT(iface, VTable) VTABLE99(implementor, iface)

#define IFACE99_PRIV_IS_MARKER_IFACE(iface) ML99_VARIADICS_IS_SINGLE((iface##_INTERFACE))
#define IFACE99_PRIV_IS_SUB_IFACE(iface)    ML99_IS_TUPLE(iface##_EXTENDS)
#define IFACE99_PRIV_IFN_LIST(iface)        ML99_VARIADICS_TAIL((iface##_INTERFACE))

#define iFn99(ret_ty, name, ...) ), (ret_ty, name, __VA_ARGS__) IFACE99_PRIV_EAT_INTERLEAVED_SEMICOLON ML99_LPAREN()

#define IFACE99_PRIV_EAT_INTERLEAVED_SEMICOLON ML99_EMPTY

#define IFACE99_PRIV_genDummy(iface)                                                               \
    ML99_IF(IFACE99_PRIV_IS_EMPTY_VTABLE(iface), v(char dummy;), ML99_empty())
#define IFACE99_PRIV_genDummyInit(iface)                                                           \
    ML99_IF(IFACE99_PRIV_IS_EMPTY_VTABLE(iface), v(.dummy = '\0'), ML99_empty())

#define IFACE99_PRIV_IS_EMPTY_VTABLE(iface)                                                        \
    ML99_AND(IFACE99_PRIV_IS_MARKER_IFACE(iface), ML99_NOT(IFACE99_PRIV_IS_SUB_IFACE(iface)))

#define dyn99(implementor, iface, ...)                                                             \
    ((iface){.self = (void *)(__VA_ARGS__), .vptr = &VTABLE99(implementor, iface)})

#define VTABLE99(implementor, iface) ML99_CAT4(implementor, _, iface, _impl)

// Arity specifiers {
#define IFACE99_PRIV_genFnPtr_ARITY           1
#define IFACE99_PRIV_genImplFnName_ARITY      2
#define IFACE99_PRIV_genRequirement_ARITY     1
#define IFACE99_PRIV_genRequirementImpl_ARITY 2

// Public:
#define IFACE99_interface_ARITY   1
#define IFACE99_impl_ARITY        2
#define IFACE99_implPrimary_ARITY 2
// }

#endif // INTERFACE99_H
