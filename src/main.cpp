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
	// IMU.reset(true);
	pros::lcd::initialize();
}

void disabled() 
{
	pros::lcd::clear();
}

void competition_initialize() {
}

void autonomous() {

}

void opcontrol() {
	// hang.set_value(1);
	// pros::Task kicker_task(kicker_trigger);
	// pros::Task hanger_task(hang_trigger);
	pros::Task driver_options_task(driver_options);
	while (true) {
		drive_robot();
		handle_intake();
		// handle_kicker();
		// handle_wings();
		// handle_hang();
		pros::delay(16);
	}        
}
