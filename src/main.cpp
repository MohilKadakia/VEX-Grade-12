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
	pros::lcd::clear();
	reset_inertial();
	while (1){
		pros::lcd::set_text(0, "Auto");
		pros::delay(10);
	}
}

void opcontrol() {
    pros::lcd::clear();
	reset_inertial();
    pros::Task drive_task(drive_robot);
	pros::Task catapult_task(catapult_trigger);
	pros::Task wings_task(wings_trigger);
	//pros::Task catapult_task(debug_values);
	while (true) {
		handle_catapult();
		handle_wings();
        pros::delay(10);
    }	
}