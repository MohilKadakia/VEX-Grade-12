#include "main.h"
#include "header/devices.hh"
#include "header/ports.h"
#include "header/functions.hh"
#include <string>

void initialize()
{
	pros::lcd::initialize();
	reset_inertial();
}

void disabled() 
{
	pros::lcd::clear();
	while (true){
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
	while (true){
		pros::lcd::set_text(0, "Auto");
		pros::delay(10);
	}
}

void opcontrol()
{
	pros::lcd::clear();
	reset_inertial();

	pros::Task catapult_task(catapult_trigger);
	pros::Task wings_task(wings_trigger);
	pros::Task drive_task(drive);
	pros::Task debug_task(debug_values);
	while (true) 
	{
		if (catapult_active) {
			catapult_motors.move_voltage(127);
        }
		else {
			catapult_motors.move_voltage(0);
		}
		if (wings_active) {
			wings.set_value(false);
		}
		else {
			wings.set_value(true);
		}
		pros::delay(10);
    }	
}