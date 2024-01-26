#include "main.h"

#include <string>
#include <cmath>

#include "autoSelect/selection.h"
#include "header/kicker.hh"
#include "header/controls.h"
#include "header/devices.hh"
#include "header/drive.hh"
#include "header/functions.hh"
#include "header/ports.h"
#include "header/wings.hh"
#include "header/auto.hh"
#include "header/intake.hh"
#include "header/blocker.hh"


void initialize() {
	selector::init();
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

void autonomous() { // Testing for new auton functions

	if (selector::auton == 1) { 
		pros::lcd::set_text(0, "Auton for Red Side Right");
	}

	else if (selector::auton == 2) {
		pros::lcd::set_text(0, "Auton for Red Side Left");
	}

	else if (selector::auton == 3) {
		pros::lcd::set_text(0, "Test (Red)");
		turn_right_to_look_at_TEST(200);
		turn_left_to_look_at_TEST(90);
		move_forward_inertial_pid(100.0, -100.0, 100.0);
	}

	else if (selector::auton == -1) {
		pros::lcd::set_text(0, "Auton for Blue Side Right");
	}

	else if (selector::auton == -2) {
		pros::lcd::set_text(0, "Auton for Blue Side Left");
	}

	else if (selector::auton == -3) {
		pros::lcd::set_text(0, "Test (Blue)");
		turn_right_to_look_at_TEST(200);
		turn_left_to_look_at_TEST(90);
		move_forward_inertial_pid(100.0, -100.0, 100.0);		
	}

	else if (selector::auton == 0) {
		pros::lcd::set_text(0, "Auton for Skills");
	}
}

void opcontrol() {
	pros::Task drive_task(drive_robot);
	pros::Task kicker_task(kicker_trigger);
	pros::Task blocker_task(blocker_trigger);
	while (true) {
		handle_kicker();
		handle_wings();
		handle_intake();
		handle_blocker();
		pros::delay(10);
	}        
}
