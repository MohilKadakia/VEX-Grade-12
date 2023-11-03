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
	pros::lcd::set_text(1, "Inititalizing v1");
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
    
    pros::Task wings_pneumatic_task(wings_pneumatic);
    pros::Task drive_task(drive_robot);
	pros::Task catapult_task(catapult_trigger);
	pros::Task debug_task(debug_values);

	while (true) {
        if (catapult_active) {
                catapult_motor.move_absolute(1000.0, 600);
        }

		// Check A button to toggle firing
        if (master.get_digital(WINGS_CONTROL)) {
            wings_active = !wings_active;
            while (master.get_digital(WINGS_CONTROL)) {
                pros::delay(10);
            }
        }

        pros::delay(10);
    }	
}