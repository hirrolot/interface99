#include <interface99.h>

#include "../common.h"
#include "types.h"

#define Foo1Impl_Foo1_a a1_impl
externImpl(Foo1, Foo1Impl);

#define Foo2Impl_a a1_impl
#define Foo2Impl_b b1_impl
externImplPrimary(Foo2, Foo2Impl);
