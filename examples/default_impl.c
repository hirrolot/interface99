// This examples demonstrates default method implementations.

#include <interface99.h>

#include <stdio.h>

#define Droid_INTERFACE                                                                            \
    method(const char *, name, void)                                                               \
    defaultMethod(void, turn_on, Droid self)

interface(Droid);

// The default implementation of `Droid::turn_on`.
void Droid_turn_on(Droid droid) {
    // A default implementation can access other methods through an interface object.
    printf("Turning on %s...\n", droid.vptr->name());
}

// C-3PO implementation
// ============================================================

typedef struct {
    char dummy;
} C_3PO;

const char *C_3PO_name(void) { return "C-3PO"; }

// Use the default implementation of `Droid::turn_on` automatically.
impl(Droid, C_3PO);

// R2-D2 implementation
// ============================================================

typedef struct {
    char dummy;
} R2_D2;

const char *R2_D2_name(void) { return "R2-D2"; }

void R2_D2_turn_on(Droid droid) {
    // A default implementation can be called from a custom one.
    Droid_turn_on(droid);
    puts("Waaaaoow!");
}

// Use a custom implementation for `Droid::turn_on`.
#define R2_D2_turn_on_CUSTOM ()
impl(Droid, R2_D2);

// Test
// ============================================================

/*
 * Output:
 * Turning on C-3PO...
 * Turning on R2-D2...
 * Waaaaoow!
 */
int main(void) {
    Droid c_3po = DYN(C_3PO, Droid, &(C_3PO){0});
    c_3po.vptr->turn_on(c_3po);

    Droid r2_d2 = DYN(R2_D2, Droid, &(R2_D2){0});
    r2_d2.vptr->turn_on(r2_d2);
}
