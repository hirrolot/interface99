// This examples demonstrates the basic usage of Interface99.

#include <interface99.h>

#include <stdio.h>

// clang-format off
#define Shape_IFACE                      \
    vfunc( int, perim, const VSelf)      \
    vfunc(void, scale, VSelf, int factor)
// clang-format on

interface(Shape);

// Rectangle implementation
// ============================================================

typedef struct {
    int a, b;
} Rectangle;

int Rectangle_perim(const VSelf) {
    VSELF(const Rectangle);
    return (self->a + self->b) * 2;
}

void Rectangle_scale(VSelf, int factor) {
    VSELF(Rectangle);
    self->a *= factor;
    self->b *= factor;
}

impl(Shape, Rectangle);

// Triangle implementation
// ============================================================

typedef struct {
    int a, b, c;
} Triangle;

int Triangle_perim(const VSelf) {
    VSELF(const Triangle);
    return self->a + self->b + self->c;
}

void Triangle_scale(VSelf, int factor) {
    VSELF(Triangle);
    self->a *= factor;
    self->b *= factor;
    self->c *= factor;
}

impl(Shape, Triangle);

// Test
// ============================================================

void test(Shape shape) {
    printf("perim = %d\n", VCALL(shape, perim));
    VCALL(shape, scale, 5);
    printf("perim = %d\n", VCALL(shape, perim));
}

/*
 * Output:
 * perim = 24
 * perim = 120
 * perim = 60
 * perim = 300
 */
int main(void) {
    Shape r = DYN_LIT(Rectangle, Shape, {5, 7});
    Shape t = DYN_LIT(Triangle, Shape, {10, 20, 30});

    test(r);
    test(t);

    return 0;
}
