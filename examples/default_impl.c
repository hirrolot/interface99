// This examples demonstrates default operation implementations.

#include <interface99.h>

#include <stdio.h>

typedef struct {
    const char *name, *description;
} Command;

#define Droid_INTERFACE(OP, ...)                                                                   \
    OP(__VA_ARGS__, const char *, name, void)                                                      \
    OP(__VA_ARGS__, const Command *, commands, void)                                               \
    OP(__VA_ARGS__, void, turn_on, Droid self)

#define Droid_turn_on_DEFAULT ()

interface(Droid);

// The default implementation of `Droid::turn_on`.
void Droid_turn_on(Droid droid) {
    // A default implementation can access other operations through an interface object.
    printf("Turning on %s...\n", droid.vptr->name());
}

// C-3PO implementation {

typedef struct {
    char dummy;
} C_3PO;

#define C_3PO_new() (&(C_3PO){0})

const char *C_3PO_name(void) { return "C-3PO"; }

const Command *C_3PO_commands(void) {
    static const Command commands[] = {
        {"translate", "Translate between over six million languages"},
        {"news", "Inform about the most recent news"},
        {NULL, NULL},
    };

    return commands;
}

// Use the default implementation of `Droid::turn_on` automatically.
impl(Droid, C_3PO);
// } (C-3PO implementation)

// R2-D2 implementation {

typedef struct {
    char dummy;
} R2_D2;

#define R2_D2_new() (&(R2_D2){0})

const char *R2_D2_name(void) { return "R2-D2"; }

const Command *R2_D2_commands(void) {
    static const Command commands[] = {
        {"fight", "Fight surrounding enemies"},
        {"send_telegram", "Send a telegram to another planet"},
        {"open_lock", "Open any door lock"},
        {NULL, NULL},
    };

    return commands;
}

void R2_D2_turn_on(Droid droid) {
    // A default implementation can be called from a custom one.
    Droid_turn_on(droid);
    puts("Waaaaoow!");
}

// Use a custom implementation for `Droid::turn_on`.
#define R2_D2_turn_on_CUSTOM ()
impl(Droid, R2_D2);
// } (R2-D2 implementation)

void test(Droid droid) {
    droid.vptr->turn_on(droid);

    printf("\n%s's available commands:\n", droid.vptr->name());

    for (const Command *cmd = droid.vptr->commands(); cmd->name != NULL; cmd++) {
        printf("/%s -- %s\n", cmd->name, cmd->description);
    }
}

/*
 * Output:
 * Turning on C-3PO...
 *
 * C-3PO's available commands:
 * /translate -- Translate between over six million languages
 * /news -- Inform about the most recent news
 *
 * ==============================
 *
 * Turning on R2-D2...
 * Waaaaoow!
 *
 * R2-D2's available commands:
 * /fight -- Fight surrounding enemies
 * /send_telegram -- Send a telegram to another planet
 * /open_lock -- Open any door lock
 */
int main(void) {
    Droid c_3po = DYN(C_3PO, Droid, C_3PO_new());
    test(c_3po);

    puts("\n==============================\n");

    Droid r2_d2 = DYN(R2_D2, Droid, R2_D2_new());
    test(r2_d2);
}
