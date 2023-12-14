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

// void initialize()
// {
// 	pros::lcd::initialize();
// 	pros::lcd::set_text(0, "Inititalizing v1");
// 	reset_inertial();
// }

void hold(double holdValue, double holdTime){
	double startTime = pros::millis();
	double upperLimit = startTime+(holdTime*1000);
	while (pros::millis() < upperLimit){
		pros::lcd::set_text(0, std::to_string(std::clamp((upperLimit-pros::millis())/1000, holdValue, 127.0)*(holdValue*holdTime)));
		master.set_text(0 , 0, std::to_string(std::clamp((upperLimit-pros::millis())/1000, holdValue, 127.0)*(holdValue*holdTime)));
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
	reset_inertial();
}
void autonomous() {


	hold(32, 6);
	// //Defense Side
	// left_motors.move(-83);
	// right_motors.move(-127);
	// pros::delay(1000);
	// left_motors.move(0);
	// right_motors.move(0);
	// pros::delay(500);
	// left_motors.move(65);
	// right_motors.move(127);
	// pros::delay(100);
	// wings.set_value(1);
	// pros::delay(700);
	// wings.set_value(0);
	// left_motors.move(0);
	// right_motors.move(0);
	// pros::delay(100);

	// move_forward_inertial(147, 25, 75, 50);

	// pros::delay(100);
	// left_motors.move(110);
	// right_motors.move(80);
	// wings.set_value(1);
	// pros::delay(400);
	// wings.set_value(0);
	// pros::delay(1000);
	// left_motors.move(0);
	// right_motors.move(0);
	
	// intake.set_value(1);
	// intake.set_value(0);

	// move_backward(80.0, -50.0, 50.0);
	// turn_right_to_look_at(5);
	// move_forward_inertial(100.0, -100.0, 100.0, 80.0);
	// move_backward(150.0, -100.0, 100.0);
	// move_forward_inertial(300.0, -100.0, 100.0, 80.0);
	// turn_right_to_look_at(45);

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
