#include "main.h"
#include <string>

// Left Side Motors
pros::Motor left_motor_1(1, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor left_motor_2(2, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor left_motor_3(3, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor_Group left_motors({left_motor_1, left_motor_2, left_motor_3});

pros::Motor right_motor_1(4, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_motor_2(7, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_motor_3(6, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor_Group right_motors({right_motor_1, right_motor_2, right_motor_3});

void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
		pros::delay(1);
		pros::Motor motor1(1, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES);
		motor1.move(100);
		pros::delay(20000);	
	} else {
		pros::lcd::clear_line(2);
	}
}

void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Inititalizing v1");
	pros::lcd::register_btn1_cb(on_center_button);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

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
