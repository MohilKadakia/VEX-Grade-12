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

// void initialize()
// {
// 	pros::lcd::initialize();
// 	pros::lcd::set_text(0, "Inititalizing v1");
// 	reset_inertial();
// }

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

	move_forward_inertial(150, -60.0, 60.0, 50.0);
	master.set_text(0, 0, std::to_string((IMU[0].get_yaw() + IMU[1].get_yaw())/2));
	// left side
	// push triball into net
	// move_forward(168, -50.0, 50.0);
	// pros::delay(500);
	// move_forward(28, -20.0, 20.0);
	// wings.set_value(1);
	// turn_left_to_look_at(-82.0);
	// move_forward(70, -70.0, 70.0);
	// move_backward(50, -70.0, 70.0);

	// pros::delay(500);
	// move_forward(90, -70.0, 70.0);
	// move_backward(50, -70.0, 70.0);

	// wings.set_value(0);
	// turn_right_to_look_at(0);
	// move_backward(180, -90.0, 90.0);
	// pros::delay(500);
	// turn_right_to_look_at(90);
	// pros::delay(500);
	// movmmmme_forward(65, -100.0, 100.0);

	// right side code
	// move_forward(170, -50.0, 50.0);
	// pros::delay(500);
	// move_forward(25, -20.0, 20.0);
	// wings.set_value(1);
	// turn_right_to_look_at(84.0);
	// move_forward(60, -70.0, 70.0);
	// move_backward(50, -70.0, 70.0);
	// pros::delay(500);
	// move_forward(80, -70.0, 70.0);
	// move_backward(30, -70.0, 70.0);

	// wings.set_value(0);
	// turn_left_to_look_at(0);
 	// move_backward(180, -90.0, 90.0);                                     
	// pros::delay(500);
	// turn_left_to_look_at(-90);
	// pros::delay(500);
	// end


	// skills must be on left
	// push triball into net
	// move_forward(170, -50.0, 50.0);
	// pros::delay(500);
	// move_forward(25, -20.0, 20.0);
	// wings.set_value(1);
	// turn_left_to_look_at(-82.0);
	// move_forward(60, -70.0, 70.0);
	// move_backward(50, -70.0, 70.0);
	// pros::delay(500);
	// move_forward(70, -70.0, 70.0);
	// move_backward(30, -70.0, 70.0);

	// // move back to shoot catapult
	// wings.set_value(0);
	// turn_right_to_look_at(0);
	// move_backward(80, -100.0, 100.0);
	// turn_right_to_look_at(45.0);
	// move_backward(50, -90.0, 90.0);

	// shoot catapult for 25s, then slow down
	// catapult_motors.move(87);
	// pros::delay(25000);
	// catapult_motors.move_velocity(5);

	// // go to other zone/side
	// move_forward(10, -10.0, 10.0);
	// turn_right_to_look_at(90.0);
	// move_forward(200, -100.0, 100.0);
	// catapult_motors.move_velocity(0);

	// // push triballs from right into net
	// turn_left_to_look_at(0.0);
	// wings.set_value(1);
	// move_forward(100, -100.0, 100.0);
	// move_backward(100, -100.0, 100.0);
	// wings.set_value(0);

	// // go to the front of the net and push triballs in
	// turn_left_to_look_at(45);
	// move_forward(200, -100.0, 100.0);
	// turn_right_to_look_at(90);
	// move_forward(100, -100.0, 100.0);
}

void opcontrol() {
    pros::Task drive_task(drive_robot);
	pros::Task catapult_task(catapult_trigger);
	pros::Task wings_task(wings_trigger);
	while (true) {
		handle_catapult();
		handle_wings();
        pros::delay(10);
	}        
}
