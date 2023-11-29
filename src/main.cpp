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

void initialize()
{
	pros::lcd::initialize();
	pros::lcd::set_text(0, "Inititalizing v1");
	reset_inertial();
}

void disabled() 
{
	pros::lcd::clear();
	while (1){
		pros::lcd::set_text(0, "Disabled");
		pros::delay(10);
	}
}

void competition_initialize()
{
	reset_inertial();
}

void autonomous()
{
	pros::lcd::set_text(0, "Entered Autonomous Mode");
	reset_inertial();

	move_backward(30);

	// move(130);
	// wings.set_value(1);
	// turn_left_to_look_at(-90);
	// move(50);

	pros::lcd::set_text(0, "Turn Completed");

	// pros::lcd::clear();
	// // reset_inertial();
	// // turn(180);
	// left_motors.move_absolute(2370, 100);
	// right_motors.move_absolute(2370, 100);
	// // 2313eu = 1m (Theoretical)
	// // 2370 ~= 1m
}

void opcontrol() {
    pros::lcd::clear();
	reset_inertial();
    pros::Task drive_task(drive_robot);
	pros::Task catapult_task(catapult_trigger);
	pros::Task wings_task(wings_trigger);
	pros::Task intake_task(intake_trigger);
	//pros::Task catapult_task(debug_values);
	while (true) {
		handle_catapult();
		handle_wings();
		handle_intake();
        pros::delay(10);
    }	
}