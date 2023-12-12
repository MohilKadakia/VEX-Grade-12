#include "main.h"

#include <string>
#include <cmath>

#include "header/catapult.hh"
#include "header/controls.h"
#include "header/devices.hh"
#include "header/drive.hh"
#include "header/functions.hh"
#include "header/ports.h"
#include "header/wings.hh"
#include "header/auto.hh"
#include "header/intake.hh"

// void initialize()
// {
// 	pros::lcd::initialize();
// 	pros::lcd::set_text(0, "Inititalizing v1");
// 	reset_inertial();
// }

void disabled() 
{
	pros::lcd::clear();
	while (1){
		pros::lcd::set_text(0, "Disabled");
		pros::delay(10);
	}
}

void competition_initialize() {
	reset_inertial();
}
void autonomous() {
	right_motors.move(80);
	left_motors.move(110);
	pros::delay(1500);
	left_motors.move(0);
	right_motors.move(0);

	move_backward(70.0, -50.0, 50.0);
	turn_right_to_look_at(45);
	move_backward(50.0, -90.0, 90.0);
	left_motors.move(25);
	// turn_right_to_look_at(80);
	pros::delay(600);
	left_motors.move(0);

	catapult_motors.move(87);
	pros::delay(500);
	uint32_t current_time = pros::millis();
	while(pros::millis() < current_time + 30000) {
		catapult_motors.move(87);
		if(abs(catapult_motor_1.get_actual_velocity()) < 1) {
			catapult_motors.move(0);
			pros::delay(400);
			catapult_motors.move(87);
			pros::delay(1000);
		}
		pros::delay(10);
	}
	// pros::delay(30000);
	// catapult_motors.move(0);
	// pros::delay(5000);
	move_forward_inertial(20.0, -50.0, 50.0, 40.0);
	reset_inertial();
	turn_right_to_look_at(90);
	move_forward_inertial(120.0, -50.0, 50.0, 40.0);
	turn_left_to_look_at(45); // this may require tweaking 
	catapult_motors.move(100);
	move_forward_inertial(500.0, -100.0, 100.0, 80.0);
	catapult_motors.move(0);
	turn_left_to_look_at(-45);
}

void opcontrol() {
    pros::Task drive_task(drive_robot);
	pros::Task catapult_task(catapult_trigger);
	pros::Task wings_task(wings_trigger);
	pros::Task intake_task(intake_trigger);
	while (true) {
		handle_catapult();
		handle_wings();
		handle_intake();
        pros::delay(10);
	}        
}
