#include "devices.hh"
#include <iostream>

double acceleration[2];
extern double velocity[2];
extern double position[2];
double previous_time;

void get_acceleration() {
    pros::c::imu_accel_s_t accel = IMU.get_accel();
    pros::lcd::set_text(0, std::to_string(accel.z));
}

void reset_position() {
    velocity[0] = 0;
    velocity[1] = 0;
    acceleration[0] = 0;
    acceleration[1] = 0;
    position[0] = 0;
    position[1] = 0;
    previous_time = (int) (pros::millis) /1000;
}

void get_position() {   
    pros::c::imu_accel_s_t accel = IMU.get_accel();
    acceleration[0] = accel.z;   
    
    double current_time = (int) (pros::millis) /1000;
    double delta_time = current_time - previous_time;
    velocity[0] += acceleration[0] * delta_time;
    position[0] += velocity[0] * delta_time;
    velocity[1] += acceleration[1] * delta_time;
    position[1] += velocity[1] * delta_time;
    previous_time = current_time;
}