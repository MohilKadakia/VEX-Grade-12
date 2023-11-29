#ifndef _devices_HH_
#define _devices_HH_
#include "main.h"
//controllers
extern pros::Controller master;
//motors
extern pros::Motor left_motor_1;
extern pros::Motor left_motor_2;
extern pros::Motor left_motor_3;
extern pros::Motor right_motor_1;
extern pros::Motor right_motor_2;
extern pros::Motor right_motor_3;

extern pros::Motor_Group left_motors;
extern pros::Motor_Group right_motors;
extern pros::Motor_Group catapult_motors;

extern pros::Motor catapult_motor_1;
extern pros::Motor catapult_motor_2;

extern pros::IMU IMU[];
extern pros::ADIDigitalOut wings;
extern pros::ADIDigitalOut intake;
#endif