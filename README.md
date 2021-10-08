# Interface99
[![CI](https://github.com/Hirrolot/interface99/workflows/C/C++%20CI/badge.svg)](https://github.com/Hirrolot/interface99/actions)
[![mailing list](https://img.shields.io/badge/mailing%20list-lists.sr.ht-orange)](https://lists.sr.ht/~hirrolot/metalang99)

Type-safe zero-boilerplate interfaces for pure C99, implemented as a single-header library.

[ [`examples/state.c`](examples/state.c) ]
```c
#include <interface99.h>

#include <stdio.h>

#define State_IFACE                      \
    iMethod( int, get, void *self)       \
    iMethod(void, set, void *self, int x)

interface(State);

typedef struct {
    int x;
} Num;

int  Num_get(void *self) { return ((Num *)self)->x; }
void Num_set(void *self, int x) { ((Num *)self)->x = x; }

impl(State, Num);

void test(State st) {
    printf("x = %d\n", st.vptr->get(st.self));
    st.vptr->set(st.self, 5);
    printf("x = %d\n", st.vptr->get(st.self));
}

int main(void) {
    Num n = {0};
    State st = DYN(Num, State, &n);
    test(st);
}
```

<details>
  <summary>Output</summary>

```
x = 0
x = 5
```

</details>

The design of Interface99 is pretty similar to that of high-level programming languages; at the same time, it feels like you still program in plain C. Just functions and data structures, nothing more.

## Highlights

 - **Zero-boilerplate.** Forget about constructing virtual tables manually -- Interface99 will do it for you!

 - **Portable.** Everything you need is a standard-conforming C99 compiler; neither the standard library, nor compiler/platform-specific functionality or VLA are required.

 - **Predictable.** Interface99 comes with formal [code generation semantics], meaning that the generated data layout is guaranteed to always be the same.

 - **Comprehensible errors.** Interface99 is [resilient to bad code].

[code generation semantics]: #semantics
[resilient to bad code]: #q-what-about-compile-time-errors

## Features

| Feature | Status | Description |
|---------|--------|-------------|
| [Multiple interface inheritance](examples/read_write.c) | ✅ | A type can inherit multiple interfaces at the same time. |
| [Superinterfaces](examples/airplane.c) | ✅ | One interface can require a set of other interfaces to be implemented as well. |
| [Marker interfaces](examples/marker.c) | ✅ | An interface with no methods. |
| [Single/Dynamic dispatch](examples/state.c) | ✅ | Determine a method to be called at runtime based on `self`. |
| Multiple dispatch | ❌ | Determine a method to be called at runtime based on multiple arguments. Likely to never going to be implemented. |
| [Dynamic objects of multiple interfaces](examples/read_write_both.c)  | ✅ | Given interfaces `Foo` and `Bar`, you can construct an object of both interfaces, `FooBar obj`. |
| [Default implementations](examples/default_impl.c)  | ✅ | Some interface methods may be given default implementations. A default method can call other methods and vice versa. |

## Installation

 1. Download Interface99 and [Metalang99] (minimum supported version -- [1.10.0](https://github.com/Hirrolot/metalang99/releases/tag/v1.10.0)).
 2. Add `interface99` and `metalang99/include` to your include paths.
 3. `#include <interface99.h>` beforehand.

Some handy advices:

 - **PLEASE**, use Interface99 only with [`-ftrack-macro-expansion=0`] (GCC), [`-fmacro-backtrace-limit=1`] (Clang), or something similar, otherwise it will throw your compiler to the moon.

 - [Precompile headers] that use Interface99 so that they will not be compiled each time they are included. It is helpful to reduce compilation times, but they are not mandatory.

[Metalang99]: https://github.com/Hirrolot/metalang99
[Precompile headers]: https://en.wikipedia.org/wiki/Precompiled_header
[`-ftrack-macro-expansion=0`]: https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html
[`-fmacro-backtrace-limit=1`]: https://clang.llvm.org/docs/ClangCommandLineReference.html#cmdoption-clang-fmacro-backtrace-limit

## Tutorial

This section is based on a collection of well-documented [examples](examples/), each of which demonstrates one specific aspect of Interface99.

### Basic usage

Interface99 lies upon these three basic concepts:

 1. **Interface definition:**

[`interface(Vehicle);`](#interface)

(This is much like defining a `struct`/`union`/`enum`. Usually it goes in `*.h` files.)

 2. **Implementation declaration:**

| Linkage | Syntax |
|---------|--------|
| Internal | [`declImpl(Vehicle, Car);`](#declImpl) |
| External | [`externDeclImpl(Vehicle, Car);`](#externDeclImpl) |

(If your interface implementation is to be exposed to other TUs, you can write `externDeclImpl(...)` in a `*.h` file and `externImpl` (see below) in a corresponding `*.c` file.)

 3. **Implementation definition:**

| Linkage | Syntax |
|---------|--------|
| Internal | [`impl(Vehicle, Car);`](#impl) |
| External | [`externImpl(Vehicle, Car);`](#externImpl) |

(This is the actual place where a certain interface is implemented for a certain type.)

Now what do the macros generate? `interface` generates a virtual table and a so-called _interface object_ type. In the case of [`examples/state.c`](examples/state.c):

```c
typedef struct StateVTable {
    int (*get)(void *self);
    void (*set)(void *self, int x);
} StateVTable;

typedef struct State {
    void *self;
    const StateVTable *vptr;
} State;
```

`impl` generates a constant variable of type `StateVTable`:

```c
static const StateVTable Num_State_impl = {
    .get = Num_get,
    .set = Num_set,
};
```

(If you were using [`externImpl`](#externImpl), this definition would be `extern` likewise.)

This is the implementation of `State` for `Num`: it contains all the methods needed to satisfy the interface. Normally, you will not use it directly but through an interface object of type `State`:

 - `State.self` is the pointer to an object whose type implements `State` (in our case -- `Num`).
 - `State.vptr` is the pointer to an implementer's virtual table (`Num_State_impl`).

`State`, in its turn, is instantiated by [`DYN`](#DYN):

```с
Num n = {0};
State st = DYN(Num, State, &n);
```

Since `State` is polymorphic over its implementations (which is the essence of dynamic dispatch), you can accept it as a function parameter and manipulate it through `.self` & `.vptr` like this:

```c
void test(State st) {
    printf("x = %d\n", st.vptr->get(st.self));
    st.vptr->set(st.self, 5);
    printf("x = %d\n", st.vptr->get(st.self));
}
```

... and this is all you need to know to write most of the stuff.

### Superinterfaces

Interface99 has the feature called superinterfaces, or interface requirements. [`examples/airplane.c`](examples/airplane.c) demonstrates how to extend interfaces with new functionality:

```c
#define Vehicle_IFACE                                     \
    iMethod(void, move_forward, void *self, int distance) \
    iMethod(void, move_back, void *self, int distance)

interface(Vehicle);

#define Airplane_IFACE                                \
    iMethod(void, move_up, void *self, int distance)  \
    iMethod(void, move_down, void *self, int distance)

#define Airplane_EXTENDS (Vehicle)

interface(Airplane);
```

(Note that `#define Airplane_EXTENDS` must appear prior to `interface(Airplane);`.)

Here, `Airplane` extends `Vehicle` with the new methods `move_up` and `move_down`. Everywhere you have `Airplane`, you also have a pointer to `VehicleVTable` accessible as `Airplane.vptr->Vehicle`:

```c
Airplane my_airplane = DYN(MyAirplane, Airplane, &(MyAirplane){0, 0});

my_airplane.vptr->Vehicle->move_forward(my_airplane.self, 10);
my_airplane.vptr->Vehicle->move_back(my_airplane.self, 3);
```

Thus, Interface99 embeds superinterfaces' virtual tables into those of subinterfaces, thereby forming a _virtual table hierarchy_. Of course, you can specify an arbitrary amount of interfaces along with `(Vehicle)`, like `Repairable` and `Armoured`, and they all will be included into `AirplaneVTable` like so:

```c
// #define Airplane_EXTENDS (Vehicle, Repairable, Armoured)
typedef struct AirplaneVTable {
    void (*move_up)(void *self, int distance);
    void (*move_down)(void *self, int distance);
    const VehicleVTable *Vehicle;
    const RepairableVTable *Repairable;
    const ArmouredVTable *Armoured;
} AirplaneVTable;
```

### Default implementations

Sometimes we wish to define default behaviour for several implementers; this is supported by _default implementations_.

Take a look at [`examples/default_impl.c`](examples/default_impl.c). In this example, we define the interface `Droid`:

```c
#define Droid_IFACE                          \
    iMethod(const char *, name, void)        \
    defaultIMethod(void, turn_on, Droid self)

interface(Droid);
```

The macro `defaultIMethod` tells Interface99 to use the default implementation for `turn_on` automatically. But where is it located? Here:

```c
void Droid_turn_on(Droid droid) {
    printf("Turning on %s...\n", droid.vptr->name());
}
```

As you can see, default implementations follow a strict naming convention, `<iface>_<default-method-name>` -- this provides Interface99 with sufficient information to generate a virtual table. For `C_3PO`, we use the default implementation of `turn_on`, and the resulting virtual table would look like this:

```c
static const DroidVTable C_3PO_Droid_impl = {
    .name = C_3PO_name,
    .turn_on = Droid_turn_on,
};
```

But for `R2_D2`, we use a custom implementation `R2_D2_turn_on`:

```c
void R2_D2_turn_on(Droid droid) {
    Droid_turn_on(droid);
    puts("Waaaaoow!");
}

#define R2_D2_turn_on_CUSTOM ()
impl(Droid, R2_D2);
```

(`R2_D2_turn_on_CUSTOM` tells Interface99 to use the custom implementation instead of the default one; this is because it is impossible to detect at compile-time whether a specific function is defined or not.)

And the virtual table would be:

```c
static const DroidVTable R2_D2_Droid_impl = {
    .name = R2_D2_name,
    .turn_on = R2_D2_turn_on,
};
```

Please, note that you have to specify `()` for the `*_CUSTOM` attribute; do not leave it empty.

Happy hacking!

## Syntax and semantics

Having a well-defined semantics of the macros, you can write an FFI which is quite common in C.

### EBNF syntax

```ebnf
<iface-def>      ::= "interface(" <iface> ")" ;

<method>         ::= <regular-method> | <default-method> ;
<regular-method> ::= "iMethod("        <method-ret-ty> "," <method-name> "," <method-params> ")" ;
<default-method> ::= "defaultIMethod(" <method-ret-ty> "," <method-name> "," <method-params> ")" ;
<method-ret-ty>  ::= <type> ;
<method-name>    ::= <ident> ;
<method-params>  ::= <parameter-type-list> ;

<impl>           ::= "impl("           <iface> "," <implementer> ")" ;
<externImpl>     ::= "externImpl("     <iface> "," <implementer> ")" ;
<declImpl>       ::= "declImpl("       <iface> "," <implementer> ")" ;
<externDeclImpl> ::= "externDeclImpl(" <iface> "," <implementer> ")" ;

<dyn>            ::= "DYN("    <implementer> "," <iface> "," <ptr> ")" ;
<vtable>         ::= "VTABLE(" <implementer> "," <iface> ")" ;

<iface>          ::= <ident> ;
<implementer>    ::= <ident> ;
<requirement>    ::= <iface> ;
```

Notes:

 - For every interface `<iface>`, the macro `<iface>_IFACE` must expand to `{ <method> }*`.
 - For any interface, a macro `<iface>_EXTENDS` can be defined, which must expand to `"(" <requirement> { "," <requirement> }* ")"`.
 - For any interface method implementation, a macro `<implementer>_<method-name>_CUSTOM` can be defined, which must expand to `"()"`.

[Clang-Format]: https://clang.llvm.org/docs/ClangFormatStyleOptions.html

### Semantics

(It might be helpful to look at the [generated data layout](https://godbolt.org/z/rh8Meb89E) of [`examples/state.c`](examples/state.c).)

#### `interface`

Expands to

```
typedef struct <iface>VTable <iface>VTable;
typedef struct <iface> <iface>;

struct <iface>VTable {
    // Only if <iface> is a marker interface without superinterfaces:
    char dummy;

    <method-ret-ty>0 (*<method-name>0)(<method-params>0);
    ...
    <method-ret-ty>N (*<method-name>N)(<method-params>N);

    const <requirement>0VTable *<requirement>;
    ...
    const <requirement>NVTable *<requirement>;
};

struct <iface> {
    void *self;
    const <iface>VTable *vptr;
}
```

(`char dummy;` is needed for an empty `<iface>VTable` because a structure must have at least one member, according to C99.)

I.e., this macro defines a virtual table structure for `<iface>`, as well as the structure `<iface>` that is polymorphic over `<iface>` implementers. This is generated in two steps:

 - **Methods' pointers**. For each `<method-name>I` specified in the macro `<iface>_IFACE`, the corresponding function pointer is generated.
 - **Requirements obligation.** If the macro `<iface>_EXTENDS` is defined, then the listed requirements are generated to obligate `<iface>` implementers to satisfy them.

#### `impl`

Expands to

```
static const <iface>VTable VTABLE(<implementer>, <iface>) = {
    // Only if <iface> is a marker interface without superinterfaces:
    .dummy = '\0',

    <method-name>0 = either <implementer>_<method-name>0 or <iface>_<method-name>0,
    ...
    <method-name>N = either <implementer>_<method-name>N or <iface>_<method-name>N,

    <requirement>0 = &VTABLE(<implementer>, <requirement>0),
    ...
    <requirement>N = &VTABLE(<implementer>, <requirement>N),
}
```

I.e., this macro defines a virtual table instance of type `<iface>VTable` for `<implementer>`. It is generated in two steps:

 - **Methods' implementations.** If `<method-name>I` is defined via `defaultIMethod` and `<implementer>_<method-name>I_CUSTOM` is **not** defined, `<iface>_<method-name>I` is generated (default implementation). Otherwise, `<implementer>_<method-name>I` is generated (custom implementation).
 - **Requirements satisfaction.** If the macro `<iface>_EXTENDS` is defined, then the listed requirements are generated to satisfy `<iface>`.

#### `declImpl`

Expands to `static const <iface>VTable VTABLE(<implementer>, <iface>)`, i.e., it declares a virtual table instance of `<implementer>` of type `<iface>VTable`.

#### `externImpl`

The same as [`impl`](#impl) but generates an `extern` definition instead of `static`.

#### `externDeclImpl`

The same as [`declImpl`](#declImpl) but generates an `extern` declaration instead of `static`.

#### `DYN`

Expands to an expression of type `<iface>`, with `.self` initialised to `<ptr>` and `.vptr` initialised to `&VTABLE(<implementer>, <iface>)`.

`<ptr>` is guaranteed to be evaluated only once.

#### `VTABLE`

Expands to `<implementer>_<iface>_impl`, i.e., a virtual table instance of `<implementer>` of type `<iface>VTable`.

## Miscellaneous

 - The macros `IFACE99_MAJOR`, `IFACE99_MINOR`, and `IFACE99_PATCH` stand for the corresponding components of a version of Interface99.

 -  If you do **not** want the shortened versions to appear (e.g., `interface` without the prefix `99`), define `IFACE99_NO_ALIASES` prior to `#include <interface99.h>`.

 - For each macro using `ML99_EVAL`, Interface99 provides its [Metalang99-compliant](https://metalang99.readthedocs.io/en/latest/#definitions) counterpart which can be used inside derivers and other Metalang99-compliant macros:

| Macro | Metalang99-compliant counterpart |
|----------|----------|
| `interface` | `IFACE99_interface` |
| `impl` | `IFACE99_impl` |
| `externImpl` | `IFACE99_externImpl` |

(An [arity specifier] and [desugaring macro] are provided for each of the above macros.)

[arity specifier]: https://hirrolot.gitbook.io/metalang99/partial-application
[desugaring macro]: https://metalang99.readthedocs.io/en/latest/#definitions

## Guidelines

 - Write `impl(...)`/`externImpl(...)` right after all methods are implemented; do not gather all implementation definitions in a single place.
 - If you use [Clang-Format], it can be helpful to add `iMethod` and `defaultIMethod` to the `StatementMacros` vector (see [our `.clang-format`](.clang-format)). It will instruct the formatter to place them onto different lines.

## Pitfalls

 - Both interfaces that you implement for a single type can have a method with the same name, thus resulting in a name collision. However, you can elegantly workaround like this:

```c
// `MyType_Iface1_foo` method definition here...

#define Iface1_foo MyType_Iface1_foo
impl(Iface1, MyType);
#undef Iface1_foo

// `MyType_Iface2_foo` method definition here...

#define Iface2_foo MyType_Iface2_foo
impl(Iface2, MyType);
#undef Iface2_foo
```

The same holds for custom implementations:

```c
// Use a custom implementation for `Iface1::bar`.
#define MyType_bar_CUSTOM ()
impl(Iface1, MyType);
#undef MyType_bar_CUSTOM

// Use the default `Iface2::bar`.
impl(Iface2, MyType);
```

## Credits

Thanks to Rust and Golang for their implementations of traits/interfaces.

## Publications

 - [_Comparing Rust and Interface99_](https://www.reddit.com/r/ProgrammingLanguages/comments/q40om0/comparing_interfaces_rust_and_interface99/) by Hirrolot.
 - [_What’s the Point of the C Preprocessor, Actually?_](https://hirrolot.github.io/posts/whats-the-point-of-the-c-preprocessor-actually.html) by Hirrolot.
 - [_Macros on Steroids, Or: How Can Pure C Benefit From Metaprogramming_](https://hirrolot.github.io/posts/macros-on-steroids-or-how-can-pure-c-benefit-from-metaprogramming.html) by Hirrolot.
 - [_Extend Your Language, Don’t Alter It_](https://hirrolot.github.io/posts/extend-your-language-dont-alter-it.html) by Hirrolot.

## Release procedure

 1. Update `IFACE99_MAJOR`, `IFACE99_MINOR`, and `IFACE99_PATCH` in `interface99.h`.
 2. Update `CHANGELOG.md`.
 3. Release the project in [GitHub Releases].

[GitHub Releases]: https://github.com/Hirrolot/interface99/releases

## FAQ

### Q: Why use C instead of Rust/Zig/whatever else?

A: See [Datatype99's README >>](https://github.com/Hirrolot/datatype99#q-why-use-c-instead-of-rustzigwhatever-else).

### Q: Why not third-party code generators?

A: See [Metalang99's README >>](https://github.com/Hirrolot/metalang99#q-why-not-third-party-code-generators).

### Q: How does it work?

A: Interface99 is implemented upon [Metalang99], a preprocessor metaprogramming library that allows enriching pure C with some custom syntax sugar.

### Q: Does it work on C++?

A: Yes, C++11 and onwards is supported.

### Q: How Interface99 differs from similar projects?

A:

 - **Less boilerplate.** In particular, Interface99 deduces method implementations from the context, thus improving code maintenance. To my knowledge, no other alternative can do this.

 - **Small.** Interface99 only features the software interface concept, no less and no more -- it does not bring all the other fancy OOP stuff, unlike [GObject] or [COS].

 - **Depends on Metalang99.** Interface99 is built upon [Metalang99], the underlying metaprogramming framework. With Metalang99, you can also use [Datatype99].

Other worth-mentioning projects:

 - [typeclass-interface-pattern], though it is rather a general idea than a ready-to-use implementation.
 - [OOC] -- a book about OO programming in ANSI C.

[`obj.h`]: https://github.com/small-c/obj.h
[GObject]: https://developer.gnome.org/gobject/stable/
[COS]: http://ldeniau.web.cern.ch/ldeniau/cos.html
[Datatype99]: https://github.com/Hirrolot/datatype99
[typeclass-interface-pattern]: https://github.com/TotallyNotChase/typeclass-interface-pattern
[OOC]: https://www.cs.rit.edu/~ats/books/ooc.pdf

### Q: What about compile-time errors?

#### Error: missing interface implementation

[`playground.c`]
```c
#define Foo_IFACE iMethod(void, foo, int x, int y)
interface(Foo);

typedef struct {
    char dummy;
} MyFoo;

// Missing `void MyFoo_foo(int x, int y)`.

impl(Foo, MyFoo);
```

[`/bin/sh`]
```
playground.c:12:1: error: ‘MyFoo_foo’ undeclared here (not in a function)
   12 | impl(Foo, MyFoo);
      | ^~~~
```

----------

#### Error: improperly typed interface implementation

[`playground.c`]
```c
#define Foo_IFACE iMethod(void, foo, int x, int y)
interface(Foo);

typedef struct {
    char dummy;
} MyFoo;

void MyFoo_foo(const char *str) {}

impl(Foo, MyFoo);
```

[`/bin/sh`]
```
playground.c:12:1: warning: initialization of ‘void (*)(int,  int)’ from incompatible pointer type ‘void (*)(const char *)’ [-Wincompatible-pointer-types]
   12 | impl(Foo, MyFoo);
      | ^~~~
playground.c:12:1: note: (near initialization for ‘MyFoo_Foo_impl.foo’)
```

----------

#### Error: unsatisfied interface requirement

[`playground.c`]
```c
#define Foo_IFACE   iMethod(void, foo, int x, int y)
interface(Foo);

#define Bar_IFACE   iMethod(void, bar, void)
#define Bar_EXTENDS (Foo)

interface(Bar);

typedef struct {
    char dummy;
} MyBar;

void MyBar_bar(void) {}

// Missing `impl(Foo, MyBar)`.

impl(Bar, MyBar);
```

[`/bin/sh`]
```
playground.c:19:1: error: ‘MyBar_Foo_impl’ undeclared here (not in a function); did you mean ‘MyBar_Bar_impl’?
   19 | impl(Bar, MyBar);
      | ^~~~
      | MyBar_Bar_impl
```

----------

#### Error: typo in `DYN`

[`playground.c`]
```c
#define Foo_IFACE iMethod(void, foo, void)
interface(Foo);

typedef struct {
    char dummy;
} MyFoo;

void MyFoo_foo(void) {}

impl(Foo, MyFoo);

int main(void) { Foo foo = DYN(MyFoo, /* Foo */ Bar, &(MyFoo){0}); }
```

[`/bin/sh`]
```
playground.c:14:28: error: ‘Bar’ undeclared (first use in this function)
   14 | int main(void) { Foo foo = DYN(MyFoo, /* Foo */ Bar, &(MyFoo){0}); }
      |                            ^~~
playground.c:14:28: note: each undeclared identifier is reported only once for each function it appears in
playground.c:14:31: error: expected ‘)’ before ‘{’ token
   14 | int main(void) { Foo foo = DYN(MyFoo, /* Foo */ Bar, &(MyFoo){0}); }
      |                            ~~~^
      |                               )
```

----------

#### Error: typo in `VTABLE`

[`playground.c`]
```c
#define Foo_IFACE iMethod(void, foo, void)
interface(Foo);

typedef struct {
    char dummy;
} MyFoo;

void MyFoo_foo(void) {}

impl(Foo, MyFoo);

int main(void) { FooVTable foo = VTABLE(/* MyFoo */ MyBar, Foo); }
```

[`/bin/sh`]
```
playground.c:14:34: error: ‘MyBar_Foo_impl’ undeclared (first use in this function); did you mean ‘MyFoo_Foo_impl’?
   14 | int main(void) { FooVTable foo = VTABLE(/* MyFoo */ MyBar, Foo); }
      |                                  ^~~~~~
      |                                  MyFoo_Foo_impl
```

----------

From my experience, nearly 95% of errors make sense.

If an error is not comprehensible at all, try to look at generated code (`-E`). Hopefully, the [code generation semantics] is formally defined so normally you will not see something unexpected.

### Q: What about IDE support?

![Suggestion](images/suggestion.png)

A: VS Code automatically enables suggestions of generated types but, of course, it does not support macro syntax highlightment.

### Q: Why use `void *self` instead of `T *self` in implementations?

A: This trick technically [results in UB](https://stackoverflow.com/questions/559581/casting-a-function-pointer-to-another-type); Interface99 is agnostic to method parameters (including `self`) though as it claims strict C99 conformance, all the examples are using `void *self`.

### Q: Which compilers are tested?

A: Interface99 is known to work on these compilers:

 - GCC
 - Clang
 - MSVC
 - TCC
