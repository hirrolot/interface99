// This example demonstrates the usage of superinterfaces/interface requirements.

#include <interface99.h>

#include <stdio.h>

#define Vehicle_IFACE                                                                              \
    vfunc(void, move_forward, VSelf, int distance)                                                 \
    vfunc(void, move_back, VSelf, int distance)

interface(Vehicle);

#define Airplane_IFACE                                                                             \
    vfunc(void, move_up, VSelf, int distance)                                                      \
    vfunc(void, move_down, VSelf, int distance)

#define Airplane_EXTENDS (Vehicle)

interface(Airplane);

typedef struct {
    int x, y;
} MyAirplane;

void MyAirplane_log(MyAirplane *self, const char *command, int distance) {
    printf("%s(%d): x = %d, y = %d\n", command, distance, self->x, self->y);
}

void MyAirplane_move_forward(VSelf, int distance) {
    VSELF(MyAirplane);
    self->x += distance;
    MyAirplane_log(self, "move_forward", distance);
}

void MyAirplane_move_back(VSelf, int distance) {
    VSELF(MyAirplane);
    self->x -= distance;
    MyAirplane_log(self, "move_back", distance);
}

impl(Vehicle, MyAirplane);

void MyAirplane_move_up(VSelf, int distance) {
    VSELF(MyAirplane);
    self->y += distance;
    MyAirplane_log(self, "move_up", distance);
}

void MyAirplane_move_down(VSelf, int distance) {
    VSELF(MyAirplane);

    self->y -= distance;
    MyAirplane_log(self, "move_down", distance);
}

impl(Airplane, MyAirplane);

/*
 * Output:
 * move_forward(10): x = 10, y = 0
 * move_back(3): x = 7, y = 0
 * move_up(7): x = 7, y = 7
 * move_down(11): x = 7, y = -4
 */
int main(void) {
    Airplane my_airplane = DYN_LIT(MyAirplane, Airplane, {.x = 0, .y = 0});

    VCALL_SUPER(my_airplane, Vehicle, move_forward, 10);
    VCALL_SUPER(my_airplane, Vehicle, move_back, 3);

    VCALL(my_airplane, move_up, 7);
    VCALL(my_airplane, move_down, 11);

    return 0;
}
