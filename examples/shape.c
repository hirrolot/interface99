// This examples demonstrates the basic usage of Interface99.

#include <interface99.h>

#include <stdio.h>

// clang-format off
#define Shape_IFACE                      \
    vfunc( int, perim, const VSelf)      \
    vfunc(void, scale, VSelf, int factor)
// clang-format on

interface(Shape);

// Rect implementation
// ============================================================

typedef struct {
    int a, b;
} Rect;

int Rect_perim(const VSelf) {
    VSELF(const Rect);
    return (self->a + self->b) * 2;
}

void Rect_scale(VSelf, int factor) {
    VSELF(Rect);
    self->a *= factor;
    self->b *= factor;
}

impl(Shape, Rect);

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
 * perim = 6
 * perim = 30
 */
int main(void) {
    Shape rect = DYN(Rect, Shape, &(Rect){5, 7}),
          triangle = DYN(Triangle, Shape, &(Triangle){1, 2, 3});

    test(rect);
    test(triangle);

    return 0;
}
