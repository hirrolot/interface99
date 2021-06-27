/*
 * This examples demonstrates default function implementations.
 *
 * The pattern is: just define a default implementation for some function
 * (`Connection_timeout_hint_default` below) and re-use it where necessary. This design follows the
 * philosophy of C that everything should be explicit: if you use a default implementation, it is
 * stated explicitly, and the same holds for a custom implementation.
 */

#include <interface99.h>

#include <stddef.h>
#include <stdio.h>

#define Connection_INTERFACE                                                                       \
    iFn(void, send, void *self, const char *data, size_t len);                                     \
                                                                                                   \
    /* The data sending timeout in seconds, -1 if for the infinite timeout. */                     \
    iFn(int, timeout_hint, void);

interface(Connection);

int Connection_timeout_hint_default(void) {
    return 10;
}

// MyConn1 {

typedef struct {
    char dummy;
} MyConn1;

#define MyConn1_timeout_hint Connection_timeout_hint_default

void MyConn1_send(void *self, const char *data, size_t len) {
    (void)self;
    (void)data;
    printf("Sending %zd bytes of data with timeout=%ds...\n", len, MyConn1_timeout_hint());
}

implPrimary(Connection, MyConn1);
// } (MyConn1)

// MyConn2 {

typedef struct {
    char dummy;
} MyConn2;

int MyConn2_timeout_hint(void) {
    return 5;
}

void MyConn2_send(void *self, const char *data, size_t len) {
    (void)self;
    (void)data;
    printf("Sending %zd bytes of data with timeout=%ds...\n", len, MyConn2_timeout_hint());
}

implPrimary(Connection, MyConn2);
// } (MyConn2)

void test(Connection conn) {
    conn.vptr->send(conn.self, NULL, 50);
    conn.vptr->send(conn.self, NULL, 70);
}

/*
 * Output:
 * Sending 50 bytes of data with timeout=10s...
 * Sending 70 bytes of data with timeout=10s...
 * Sending 50 bytes of data with timeout=5s...
 * Sending 70 bytes of data with timeout=5s...
 */
int main(void) {
    test(dyn(MyConn1, Connection, &(MyConn1){0}));
    test(dyn(MyConn2, Connection, &(MyConn2){0}));

    return 0;
}
