#include "main.h"

#include <string>
#include <cmath>

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
	// pros::lcd::initialize();
}

void hold(double holdValue, double holdTime, int delay){
	double startTime = pros::millis();
	double upperLimit = startTime+(holdTime*1000);
	while (pros::millis() < upperLimit){
		pros::lcd::set_text(0, std::to_string(std::clamp((upperLimit-(pros::millis()+((holdTime*1000)-delay)))/1000, 1.0, 127.0)*(holdValue)));
		master.set_text(0 , 0, std::to_string(std::clamp((upperLimit-(pros::millis()+((holdTime*1000)-delay)))/1000, 1.0, 127.0)*(holdValue)));
		kicker_motors.move(std::clamp((upperLimit-(pros::millis()+((holdTime*1000)-delay)))/1000, 1.0, 127.0)*(holdValue));
	}
}

void disabled() 
{
	pros::lcd::clear();
	while (1){
		pros::lcd::set_text(0, "Disabled");
		pros::delay(10);
	}
}

void competition_initialize() {
	//reset_inertial();
}

void autonomous() {

}

void opcontrol() {
	hang.set_value(1);
	pros::Task drive_task(drive_robot);
	pros::Task kicker_task(kicker_trigger);
	pros::Task hanger_task(hang_trigger);
	while (true) {
		handle_intake();
		handle_kicker();
		handle_wings();
		handle_hang();
		pros::delay(10);
	}        
}
