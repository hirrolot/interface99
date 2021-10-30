#include <interface99.h>

#include "../common.h"
#include "types.h"

#define FooImpl_foo foo1_impl
implExtern(Foo, FooImpl);

#define BarImpl_foo foo1_impl
#define BarImpl_bar bar1_impl
implExtern(Bar, BarImpl);
