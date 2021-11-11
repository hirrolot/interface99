// This example demonstrates how to write interface decorators.

#include <interface99.h>

#include <stdio.h>

#define Vehicle_IFACE                                                                              \
    vfunc(void, refuel, VSelf, int fuel)                                                           \
    vfunc(void, drive, VSelf)

interface(Vehicle);

// Car implementation
// ============================================================

typedef struct {
    int fuel;
} Car;

void Car_refuel(VSelf, int fuel) {
    VSELF(Car);
    self->fuel += fuel;
}

void Car_drive(VSelf) {
    VSELF(Car);
    self->fuel--;
}

impl(Vehicle, Car);

// TracingVehicle implementation
// ============================================================

typedef struct {
    Vehicle inner;
} TracingVehicle;

void TracingVehicle_refuel(VSelf, int fuel) {
    VSELF(TracingVehicle);
    printf("Vehicle.refuel(%d)\n", fuel);
    VCALL(self->inner, refuel, fuel);
}

void TracingVehicle_drive(VSelf) {
    VSELF(TracingVehicle);
    printf("Vehicle.drive()\n");
    VCALL(self->inner, drive);
}

impl(Vehicle, TracingVehicle);

// Test
// ============================================================

void test(Vehicle vehicle) {
    VCALL(vehicle, refuel, 15);
    VCALL(vehicle, drive);
}

/*
 * Output:
 * Vehicle.refuel(15)
 * Vehicle.drive()
 */
int main(void) {
    Vehicle car = DYN(Car, Vehicle, &(Car){0});
    Vehicle tracing_car = DYN(TracingVehicle, Vehicle, &(TracingVehicle){car});

    test(tracing_car);

    return 0;
}
