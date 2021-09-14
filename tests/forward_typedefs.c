#include <interface99.h>

#define Foo_INTERFACE(FN, CTX) FN(CTX, void, foo, Foo self, FooVTable vtable)

interface(Foo);

int main(void) {}
