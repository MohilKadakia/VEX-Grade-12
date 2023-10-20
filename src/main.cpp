#include "main.h"
#include "math.h"
#include <string>
#include <iostream>
#define endl '\n'
#define PORT1 1
#define PORT2 2
#define PORT3 3
#define PORT4 4
#define PORT5 7
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
pros::Motor right_motor_1(PORT4, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_motor_2(PORT5, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_motor_3(PORT6, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor_Group right_motors({right_motor_1, right_motor_2, right_motor_3});
pros::ADIDigitalOut wings('h');
pros::IMU IMU[] = {PORT9, PORT10};


void initialize() {
	pros::lcd::initialize();
	
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	for (int i = 0; i < 2; i++){
		IMU[i].reset();
	}
	while(true){
		for (int i = 0; i < 2; i++){
			pros::lcd::set_text(i, std::to_string(IMU[i].get_yaw()));
		}
	}
}

void opcontrol() {
	pros::lcd::set_text(1, "Enters the OPControl");
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	while (true) {
		int moveL = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) + master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
		int moveR = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) - master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
		left_motors.move(std::clamp(moveL, -127, 127));
		right_motors.move(std::clamp(moveR, -127, 127));
		pros::lcd::set_text(1, "Left: " + std::to_string(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)));
		pros::lcd::set_text(2, "Right: " + std::to_string(master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y)));
		pros::delay(10);
    }	
}