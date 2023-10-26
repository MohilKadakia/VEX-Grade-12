#include "main.h"
#include "math.h"
#include <string>
#include <iostream>
#define endl '\n'
#define PORT1 1
#define PORT2 2
#define PORT3 3
#define PORT4 4
#define PORT5 5
#define PORT6 6
#define PORT7 7
#define PORT8 8
#define PORT9 9
#define PORT10 10
#define PORT11 11
#define PORT12 12
#define PORT13 13
#define PORT14 14
#define PORT15 15
#define PORT16 16
#define PORT17 17
#define PORT18 18
#define PORT19 19
#define PORT20 20


// Left Side Motors
pros::Motor left_motor_1(PORT1, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor left_motor_2(PORT2, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor left_motor_3(PORT3, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor_Group left_motors({left_motor_1, left_motor_2, left_motor_3});
// Right Side Motors
pros::Motor right_motor_1(PORT1, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_motor_2(PORT2, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_motor_3(PORT3, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor_Group right_motors({right_motor_1, right_motor_2, right_motor_3});
// Pnemuatic Wings
pros::ADIDigitalOut wings('h');
// Inertial Sensors
pros::IMU IMU[] = {PORT9, PORT10};


void initialize() {
	pros::lcd::initialize();
	for (int i = 0; i < 2; i++){
		IMU[i].reset();
	}
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	
}
void Drive(pros::Controller master)
{
	int moveL = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) + master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
	int moveR = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) - master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
	left_motors.move(std::clamp(moveL, -127, 127));
	right_motors.move(std::clamp(moveR, -127, 127));
}
void DebugValues(pros::Controller master)
{
	pros::lcd::set_text(0, "LMtop:" + std::to_string(left_motor_1.get_encoder_units()) + " LMfro" + std::to_string(left_motor_2.get_encoder_units()) + " LMbac" + std::to_string(left_motor_3.get_encoder_units()));
	pros::lcd::set_text(1, "RMtop:" + std::to_string(right_motor_1.get_encoder_units()) + " RMfro" + std::to_string(right_motor_2.get_encoder_units()) + " RMbac" + std::to_string(right_motor_3.get_encoder_units()));
	pros::lcd::set_text(2, "ISfro" + std::to_string(IMU[0].get_yaw()) + "ISbac" + std::to_string(IMU[1].get_yaw()));


}
void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	while (true) {
		Drive(master);
		DebugValues(master);
		pros::delay(10);
    }	
}