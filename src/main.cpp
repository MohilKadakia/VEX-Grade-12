#include "main.h"
#include "header/kicker.hh"
#include "header/controls.h"
#include "header/devices.hh"
#include "header/drive.hh"
#include "header/functions.hh"
#include "header/ports.h"
#include "header/wings.hh"
#include "header/auto.hh"
#include "header/intake.hh"
#include "header/hang.hh"

void initialize()
{
	IMU.reset(true);
	pros::lcd::initialize();
}

void disabled() 
{
	pros::lcd::clear();
}

void competition_initialize() {
}

void autonomous() {
	turn(190, 1, 0.1, 0.1);
}

void opcontrol() {
	hang.set_value(1);
	left_motors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
	right_motors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
	pros::Task kicker_task(kicker_trigger);
	pros::Task hanger_task(hang_trigger);
	pros::Task wings_task(wings_trigger);
	pros::Task driver_options_task(driver_options);
	while (true) {
		drive_robot();
		handle_intake();
		handle_kicker();
		handle_wings();
		handle_hang();
		pros::delay(16);
	}        
}
