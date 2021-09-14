#include <interface99.h>

#define Foo_INTERFACE(fn, ctx) fn(ctx, void, foo, Foo self, FooVTable vtable)

interface(Foo);

int main(void) {}
