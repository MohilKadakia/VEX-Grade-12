#include "main.h"
#include "header/devices.h"
#include "header/pid.hh"
#include "header/ports.h"
#include "header/functions.hh"
#include <string>


void reset_inertial()
{
	for (int i = 0; i < 2; i++){
		IMU[i].reset();
	}
}
void drive()
{
	int moveL = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) + master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
	int moveR = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) - master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
	left_motors.move(std::clamp(moveL, -127, 127));
	right_motors.move(std::clamp(moveR, -127, 127));
}
void initialize()
{
	pros::lcd::initialize();
	ResetInertialSensors();
}
void disabled() {
	pros::lcd::clear();
	while (1){
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
	ResetInertialSensors();
	while (1){
		pros::lcd::set_text(0, "Auto");
		pros::delay(10);
	}
}

void opcontrol()
{
	pros::lcd::clear();
	ResetInertialSensors();

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