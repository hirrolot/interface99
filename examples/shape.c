// This examples demonstrates the basic usage of Interface99.

#include <interface99.h>

#include <stdio.h>

// clang-format off
#define Shape_IFACE                           \
    vfunc( int, perim, const void *self)      \
    vfunc(void, scale, void *self, int factor)
// clang-format on

interface(Shape);

// Rect implementation
// ============================================================

typedef struct {
    int a, b;
} Rect;

int Rect_perim(const void *self) {
    const Rect *this = (const Rect *)self;
    return (this->a + this->b) * 2;
}

void Rect_scale(void *self, int factor) {
    Rect *this = (Rect *)self;
    this->a *= factor;
    this->b *= factor;
}

impl(Shape, Rect);

// Triangle implementation
// ============================================================

typedef struct {
    int a, b, c;
} Triangle;

int Triangle_perim(const void *self) {
    const Triangle *this = (const Triangle *)self;
    return this->a + this->b + this->c;
}

void Triangle_scale(void *self, int factor) {
    Triangle *this = (Triangle *)self;
    this->a *= factor;
    this->b *= factor;
    this->c *= factor;
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
