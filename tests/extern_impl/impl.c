#include <interface99.h>

#include "../common.h"
#include "types.h"

#define FooImpl_foo foo1_impl
externImpl(Foo, FooImpl);

#define BarImpl_foo foo1_impl
#define BarImpl_bar bar1_impl
externImpl(Bar, BarImpl);
