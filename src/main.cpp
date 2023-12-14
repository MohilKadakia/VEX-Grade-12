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
void autonomous__() { // Auton far (left) side
	wings.set_value(1);
	left_motors.move(-110);
	right_motors.move(-80);
	wings.set_value(0);
	pros::delay(1400);
	left_motors.move(0);
	right_motors.move(0);

	pros::delay(100);
	left_motors.move(110);
	right_motors.move(80);
	wings.set_value(1);
	pros::delay(400);
	wings.set_value(0);
	pros::delay(1000);
	left_motors.move(0);
	right_motors.move(0);
}

void autonomous() { // Auton near (right) side
	wings.set_value(1);
	left_motors.move(-80);
	right_motors.move(-110);
	wings.set_value(0);
	pros::delay(800);
	left_motors.move(0);
	right_motors.move(0);

	pros::delay(100);
	wings.set_value(1);
	left_motors.move(70);
	right_motors.move(120);
	pros::delay(400);
	wings.set_value(0);
	pros::delay(700);
	left_motors.move(0);
	right_motors.move(0);
	
	move_forward(15, -20, 20);
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
