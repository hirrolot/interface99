#include <interface99.h>

#include "../common.h"
#include "../util.h"
#include "types.h"

#include <assert.h>

externDeclImpl(Foo1, Foo1Impl);

// Multiple declarations shoud work fine.
externDeclImpl(Foo2, Foo2Impl);
externDeclImpl(Foo2, Foo2Impl);

int main(void) {
    // Ensure `impl`-generated data.
    {
        assert(VTABLE(Foo1Impl, Foo1).a == a1_impl);
        assert(VTABLE(Foo2Impl, Foo2).a == a1_impl);
    }

    ENSURE_DYN_OBJ(Foo1Impl, Foo1);
    ENSURE_DYN_OBJ(Foo2Impl, Foo2);
}
