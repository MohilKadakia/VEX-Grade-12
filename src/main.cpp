#include "main.h"
#include "header/devices.h"
#include "header/pid.hh"
#include "header/ports.h"
#include "header/functions.hh"
#include <string>



void initialize()
{
	pros::lcd::initialize();
	ResetInertialSensors();
}

void disabled() 
{
	pros::lcd::clear();
	while (true)
	{
		pros::lcd::set_text(0, "Disabled");
		pros::delay(10);
	}
}

void competition_initialize()
{
	ResetInertialSensors();
}

void autonomous()
{
	pros::lcd::clear();
	reset_inertial();
	while (true)
	{
		pros::lcd::set_text(0, "Auto");
		pros::delay(10);
	}
}

void opcontrol()
{
	pros::lcd::clear();
	reset_inertial();

	pros::Task catapult_task(catapult_trigger);
	pros::Task drive_task(drive);
	pros::Task debug_task(debug_values);
	while (true)
	{
		if (catapult_active) {
			catapult_motor.move_absolute(1000.0, 600);
        }
		pros::delay(10);
    }	
}