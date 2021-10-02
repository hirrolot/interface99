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

#if !ML99_VERSION_COMPATIBLE(1, 10, 0)
#error Please, update Metalang99 to v1.10.0 or later.
#endif

#ifndef IFACE99_NO_ALIASES

#define interface      interface99
#define impl           impl99
#define declImpl       declImpl99
#define externImpl     externImpl99
#define externDeclImpl externDeclImpl99
#define method         method99
#define defaultMethod  defaultMethod99
#define DYN            DYN99
#define VTABLE         VTABLE99

#endif // IFACE99_NO_ALIASES

// Public stuff {

// Metalang99-compliant macros {

#define IFACE99_interface(iface)               ML99_call(IFACE99_interface, iface)
#define IFACE99_impl(iface, implementer)       ML99_call(IFACE99_impl, iface, implementer)
#define IFACE99_externImpl(iface, implementer) ML99_call(IFACE99_externImpl, iface, implementer)

#define interface99(iface)               ML99_EVAL(IFACE99_interface_IMPL(iface))
#define impl99(iface, implementer)       ML99_EVAL(IFACE99_impl_IMPL(iface, implementer))
#define externImpl99(iface, implementer) ML99_EVAL(IFACE99_externImpl_IMPL(iface, implementer))
// } (Metalang99-compliant macros)

#define DYN99(implementer, iface, ...)                                                             \
    ((iface){.self = (void *)(__VA_ARGS__), .vptr = &VTABLE99(implementer, iface)})
#define VTABLE99(implementer, iface) ML99_CAT4(implementer, _, iface, _impl)

#define method99(ret_ty, name, ...)        (0method, ret_ty, name, __VA_ARGS__)
#define defaultMethod99(ret_ty, name, ...) (0defaultMethod, ret_ty, name, __VA_ARGS__)

#define IFACE99_MAJOR 0
#define IFACE99_MINOR 5
#define IFACE99_PATCH 0
// } (Public stuff)

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
 * <method-ret-ty>0 (*<method-name>0)(<method-params>0);
 * ...
 * <method-ret-ty>N (*<method-name>N)(<method-params>N);
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
            IFACE99_PRIV_genMethodPtrForEach(iface)),                                              \
        ML99_IF(                                                                                   \
            IFACE99_PRIV_IS_SUB_IFACE(iface),                                                      \
            IFACE99_PRIV_genRequirementForEach(iface),                                             \
            ML99_empty())))

#define IFACE99_PRIV_genDummy(iface)                                                               \
    ML99_IF(IFACE99_PRIV_IS_EMPTY_VTABLE(iface), v(char dummy;), ML99_empty())

/*
 * <method-ret-ty>0 (*<method-name>0)(<method-params>0);
 * ...
 * <method-ret-ty>N (*<method-name>N)(<method-params>N);
 */
#define IFACE99_PRIV_genMethodPtrForEach(iface)                                                    \
    ML99_seqForEach(v(IFACE99_PRIV_genMethodPtr), v(iface##_IFACE))

#define IFACE99_PRIV_genMethodPtr_IMPL(_tag, ret_ty, name, ...) v(ret_ty (*name)(__VA_ARGS__);)

/*
 * const <requirement>0VTable *<requirement>;
 * ...
 * const <requirement>NVTable *<requirement>;
 */
#define IFACE99_PRIV_genRequirementForEach(iface)                                                  \
    ML99_tupleForEach(v(IFACE99_PRIV_genRequirement), v(iface##_EXTENDS))

#define IFACE99_PRIV_genRequirement_IMPL(requirement) v(const requirement##VTable *requirement;)
// } (Interface generation)

// Interface implementation generation {

#define IFACE99_impl_IMPL(iface, implementer)                                                      \
    IFACE99_PRIV_implCommon(IFACE99_PRIV_STORAGE_CLASS_STATIC, iface, implementer)
#define IFACE99_externImpl_IMPL(iface, implementer)                                                \
    IFACE99_PRIV_implCommon(IFACE99_PRIV_STORAGE_CLASS_EXTERN, iface, implementer)

#define IFACE99_PRIV_STORAGE_CLASS_STATIC    static
#define IFACE99_PRIV_STORAGE_CLASS_EXTERN    /* If no storage-class specifier is provided, the     \
                                                default for objects is `extern` (file scope) or    \
                                                `auto` (block scope). */

#define IFACE99_PRIV_implCommon(storage_class, iface, implementer)                                 \
    ML99_assignInitializerList(                                                                    \
        v(storage_class const iface##VTable VTABLE99(implementer, iface)),                         \
        IFACE99_PRIV_genImplInitList(iface, implementer))

/*
 * // Only if <iface> is a marker interface without superinterfaces:
 * .dummy = '\0',
 *
 * <method-name>0 = either <implementer>_<method-name>0 or <iface>_<method-name>0,
 * ...
 * <method-name>N = either <implementer>_<method-name>N or <iface>_<method-name>N,
 *
 * <requirement>0 = &VTABLE(<implementer>, <requirement>0),
 * ...
 * <requirement>N = &VTABLE(<implementer>, <requirement>N),
 */
#define IFACE99_PRIV_genImplInitList(iface, implementer)                                           \
    ML99_uncomma(ML99_QUOTE(                                                                       \
        IFACE99_PRIV_genDummyInit(iface),                                                          \
        ML99_IF(                                                                                   \
            IFACE99_PRIV_IS_MARKER_IFACE(iface),                                                   \
            ML99_empty(),                                                                          \
            IFACE99_PRIV_genImplMethodNameForEach(iface, implementer)),                            \
        ML99_IF(                                                                                   \
            IFACE99_PRIV_IS_SUB_IFACE(iface),                                                      \
            IFACE99_PRIV_genRequirementsImplForEach(iface, implementer),                           \
            ML99_empty())))

#define IFACE99_PRIV_genDummyInit(iface)                                                           \
    ML99_IF(IFACE99_PRIV_IS_EMPTY_VTABLE(iface), v(.dummy = '\0'), ML99_empty())

/*
 * <method-name>0 = either <implementer>_<method-name>0 or <iface>_<method-name>0,
 * ...
 * <method-name>N = either <implementer>_<method-name>N or <iface>_<method-name>N,
 */
#define IFACE99_PRIV_genImplMethodNameForEach(iface, implementer)                                  \
    ML99_seqForEach(                                                                               \
        ML99_appl(v(IFACE99_PRIV_genImplMethodName), v(iface, implementer)),                       \
        v(iface##_IFACE))

#define IFACE99_PRIV_genImplMethodName_IMPL(iface, implementer, tag, _ret_ty, name, ...)           \
    v(.name = ML99_IF(                                                                             \
          ML99_AND(                                                                                \
              IFACE99_PRIV_IS_DEFAULT(tag),                                                        \
              ML99_NOT(IFACE99_PRIV_IS_CUSTOM(implementer, name))),                                \
          iface##_##name,                                                                          \
          implementer##_##name), )

/*
 * <requirement>0 = &VTABLE(<implementer>, <requirement>0),
 * ...
 * <requirement>N = &VTABLE(<implementer>, <requirement>N),
 */
#define IFACE99_PRIV_genRequirementsImplForEach(iface, implementer)                                \
    ML99_tupleForEach(                                                                             \
        ML99_appl(v(IFACE99_PRIV_genRequirementImpl), v(implementer)),                             \
        v(iface##_EXTENDS))

#define IFACE99_PRIV_genRequirementImpl_IMPL(implementer, requirement)                             \
    v(.requirement = &VTABLE99(implementer, requirement), )
// } (Interface implementation generation)

// Implementation declaration {

#define declImpl99(iface, implementer) static IFACE99_PRIV_DECL_IMPL_COMMON(iface, implementer)
#define externDeclImpl99(iface, implementer)                                                       \
    extern IFACE99_PRIV_DECL_IMPL_COMMON(iface, implementer)

#define IFACE99_PRIV_DECL_IMPL_COMMON(iface, implementer)                                          \
    const ML99_CAT(iface, VTable) VTABLE99(implementer, iface)
// } (Implementation declaration)

// Various predicates {

#define IFACE99_PRIV_IS_EMPTY_VTABLE(iface)                                                        \
    ML99_AND(IFACE99_PRIV_IS_MARKER_IFACE(iface), ML99_NOT(IFACE99_PRIV_IS_SUB_IFACE(iface)))
#define IFACE99_PRIV_IS_MARKER_IFACE(iface) ML99_SEQ_IS_EMPTY(iface##_IFACE)
#define IFACE99_PRIV_IS_SUB_IFACE(iface)    ML99_IS_TUPLE(iface##_EXTENDS)

#define IFACE99_PRIV_IS_DEFAULT(tag)           ML99_DETECT_IDENT(IFACE99_PRIV_IS_DEFAULT_, tag)
#define IFACE99_PRIV_IS_DEFAULT_0defaultMethod ()

#define IFACE99_PRIV_IS_CUSTOM(implementer, method_name)                                           \
    ML99_IS_TUPLE(implementer##_##method_name##_CUSTOM)
// } (Various predicates)

// Arity specifiers {

#define IFACE99_PRIV_genRequirement_ARITY     1
#define IFACE99_PRIV_genRequirementImpl_ARITY 2

#define IFACE99_PRIV_genMethodPtr_ARITY      1
#define IFACE99_PRIV_genImplMethodName_ARITY 2

// Public:

#define IFACE99_interface_ARITY  1
#define IFACE99_impl_ARITY       2
#define IFACE99_externImpl_ARITY 2
// } (Arity specifiers)

#endif // INTERFACE99_H
