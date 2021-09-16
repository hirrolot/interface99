// This example demonstrates the usage of superinterfaces/interface requirements.

#include <interface99.h>

#include <stdio.h>

#define Vehicle_INTERFACE(OP, CTX)                                                                 \
    OP(CTX, void, move_forward, void *self, int distance)                                          \
    OP(CTX, void, move_back, void *self, int distance)

interface(Vehicle);

#define Airplane_INTERFACE(OP, CTX)                                                                \
    OP(CTX, void, move_up, void *self, int distance)                                               \
    OP(CTX, void, move_down, void *self, int distance)

#define Airplane_EXTENDS (Vehicle)

interface(Airplane);

typedef struct {
    int x, y;
} MyAirplane;

void MyAirplane_log(MyAirplane *self, const char *command, int distance) {
    printf("%s(%d): x = %d, y = %d\n", command, distance, self->x, self->y);
}

void MyAirplane_move_forward(void *self, int distance) {
    MyAirplane *this = (MyAirplane *)self;
    this->x += distance;
    MyAirplane_log(self, "move_forward", distance);
}

void MyAirplane_move_back(void *self, int distance) {
    MyAirplane *this = (MyAirplane *)self;
    this->x -= distance;
    MyAirplane_log(self, "move_back", distance);
}

impl(Vehicle, MyAirplane);

void MyAirplane_move_up(void *self, int distance) {
    MyAirplane *this = (MyAirplane *)self;
    this->y += distance;
    MyAirplane_log(self, "move_up", distance);
}

void MyAirplane_move_down(void *self, int distance) {
    MyAirplane *this = (MyAirplane *)self;

    this->y -= distance;
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
    Airplane my_airplane = DYN(MyAirplane, Airplane, &(MyAirplane){0, 0});

    my_airplane.vptr->Vehicle->move_forward(my_airplane.self, 10);
    my_airplane.vptr->Vehicle->move_back(my_airplane.self, 3);

    my_airplane.vptr->move_up(my_airplane.self, 7);
    my_airplane.vptr->move_down(my_airplane.self, 11);

    return 0;
}
