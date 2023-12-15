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
	// pros::lcd::initialize();
	// pros::lcd::set_text(0, "Inititalizing v1");
	//reset_inertial();
}

void hold(double holdValue, double holdTime, int delay){
	double startTime = pros::millis();
	double upperLimit = startTime+(holdTime*1000);
	while (pros::millis() < upperLimit){
		pros::lcd::set_text(0, std::to_string(std::clamp((upperLimit-(pros::millis()+((holdTime*1000)-delay)))/1000, 1.0, 127.0)*(holdValue)));
		master.set_text(0 , 0, std::to_string(std::clamp((upperLimit-(pros::millis()+((holdTime*1000)-delay)))/1000, 1.0, 127.0)*(holdValue)));
		catapult_motors.move(std::clamp((upperLimit-(pros::millis()+((holdTime*1000)-delay)))/1000, 1.0, 127.0)*(holdValue));
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

void autonomous() { // Auton near (right) side
	left_motors.move(-120);
	right_motors.move(-78);
	pros::delay(800);
	left_motors.move(0);
	right_motors.move(0);
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
