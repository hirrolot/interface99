# Interface99
[![CI](https://github.com/Hirrolot/interface99/workflows/C/C++%20CI/badge.svg)](https://github.com/Hirrolot/interface99/actions)

Zero-boilerplate interfaces for pure C99, implemented as a single-header library.

[ [`examples/state.c`](examples/state.c) ]
```c
#include <interface99.h>

#include <stdio.h>

#define State_INTERFACE               \
    iFn(int, get, void *self);        \
    iFn(void, set, void *self, int x);

interface(State);

typedef struct {
    int x;
} Num;

int Num_State_get(void *self) {
    return ((Num *)self)->x;
}

void Num_State_set(void *self, int x) {
    ((Num *)self)->x = x;
}

impl(State, Num);

void test(State st) {
    printf("x = %d\n", st.vptr->get(st.self));
    st.vptr->set(st.self, 5);
    printf("x = %d\n", st.vptr->get(st.self));
}

int main(void) {
    Num n = {0};
    State st = dyn(State, Num, &n);
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

No dynamic memory allocation is performed. Even the standard library is not a dependency. Everything you need is a standard-conforming C99 preprocessor.

## Installation

 1. Download Interface99 and [Metalang99] (minimum supported version -- [1.1.0](https://github.com/Hirrolot/metalang99/releases/tag/v1.1.0)).
 2. Add `interface99` and `metalang99/include` to your include paths.
 3. `#include <interface99.h>` beforehand.

**PLEASE**, use Interface99 only with [`-ftrack-macro-expansion=0`] (GCC) or something similar, otherwise it will throw your compiler to the moon. [Precompiled headers] are also very helpful.

[Metalang99]: https://github.com/Hirrolot/metalang99
[precompiled headers]: https://en.wikipedia.org/wiki/Precompiled_header
[`-ftrack-macro-expansion=0`]: https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html

## Syntax and semantics

### EBNF syntax

TODO.

### Semantics

TODO.

## Miscellaneous

TODO.

## Credits

Thanks to Rust for its implementation of traits.

## FAQ

### Q: What about IDE support?

A: TODO.

### Q: What compilers are tested?

A: Interface99 is known to work on these compilers:

 - GCC
 - Clang
 - MSVC
 - TCC
