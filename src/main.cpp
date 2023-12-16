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
	// veer to the right to push triball into net
	right_motors.move(80);
	left_motors.move(110);
	pros::delay(1500);
	left_motors.move(0);
	right_motors.move(0);

	// align with wall
	move_backward(37.0, -50.0, 50.0);
	turn_right_to_look_at(72);
	move_backward(20.0, -70.0, 70.0);

	// matchload and consistently touch roller
	left_motors.move(-10);
	right_motors.move(-10);
	catapult_motors.move(87);
	uint32_t current_time = pros::millis();
	pros::delay(500);
	while(pros::millis() < current_time + 25000) { // make 5000 into 30000
		catapult_motors.move(87);
		if(abs(catapult_motor_1.get_actual_velocity()) < 1) {
			catapult_motors.move(0);
			pros::delay(400);
			catapult_motors.move(87);
			pros::delay(1000);
		}
		pros::delay(250);
	}
	catapult_motors.move(0);

	// begin moving to other side
	move_forward_inertial(20.0, -50.0, 50.0, 40.0);
	reset_inertial();
	turn_right_to_look_at(51); // this may require tweaking
	move_forward_inertial(160.0, -100.0, 80.0, 50.0);
	turn_left_to_look_at(15);
	catapult_motors.move(100);
	move_forward_inertial(250.0, -100.0, 80.0, 70.0);
	catapult_motors.move(0);

	// push triballs into the net (right side)
	turn_left_to_look_at(-30);
	wings.set_value(1);
	move_forward_inertial(110.0, -80.0, 100.0, 90.0);
	wings.set_value(0);

	left_motors.move(60);
	right_motors.move(120);
	pros::delay(2700);
	left_motors.move(0);
	right_motors.move(0);

	move_backward(70.0, -40.0, 40.0);
	move_forward_inertial(140.0, -50.0, 127.0, 110.0);

	// push triballs front of the net
	move_backward(60.0, -40.0, 40.0);
	turn_left_to_look_at(-140);
	move_forward_inertial(300.0, -50.0, 80.0, 70.0);
	turn_right_to_look_at(5);
	wings.set_value(1);
	move_forward_inertial(175.0, -50.0, 127.0, 110.0);
	wings.set_value(0);
	move_backward(50.0, -50.0, 80.0);
	
	// push triballs left side front of net
	turn_left_to_look_at(-35);
	wings.set_value(1);
	left_motors.move(100);
	right_motors.move(80);
	pros::delay(2300);
	left_motors.move(0);
	right_motors.move(0);
	wings.set_value(0);
	move_backward(50.0, -80.0, 80.0);

	// // veer to the right to push triball into net
	// right_motors.move(80);
	// left_motors.move(110);
	// pros::delay(1500);
	// left_motors.move(0);
	// right_motors.move(0);

	// // align with wall
	// move_backward(37.0, -50.0, 50.0);
	// turn_right_to_look_at(72);
	// move_backward(20.0, -70.0, 70.0);

	// // matchload and consistently touch roller
	// left_motors.move(-10);
	// right_motors.move(-10);
	// catapult_motors.move(87);
	// uint32_t current_time = pros::millis();
	// pros::delay(500);
	// while(pros::millis() < current_time + 25000) { // make 5000 into 30000
	// 	catapult_motors.move(87);
	// 	if(abs(catapult_motor_1.get_actual_velocity()) < 1) {
	// 		catapult_motors.move(0);
	// 		pros::delay(400);
	// 		catapult_motors.move(87);
	// 		pros::delay(1000);
	// 	}
	// 	pros::delay(250);
	// }
	// catapult_motors.move(0);

	// // begin moving to other side
	// move_forward_inertial(20.0, -50.0, 50.0, 40.0);
	// reset_inertial();
	// turn_right_to_look_at(51); // this may require tweaking
	// move_forward_inertial(160.0, -100.0, 80.0, 50.0);
	// turn_left_to_look_at(15);
	// catapult_motors.move(100);
	// move_forward_inertial(250.0, -100.0, 80.0, 70.0);
	// catapult_motors.move(0);

	// // push triballs into the net (right side)
	// turn_left_to_look_at(-30);
	// wings.set_value(1);
	// move_forward_inertial(110.0, -80.0, 100.0, 90.0);
	// wings.set_value(0);

	// left_motors.move(60);
	// right_motors.move(120);
	// pros::delay(2700);
	// left_motors.move(0);
	// right_motors.move(0);

	// move_backward(70.0, -40.0, 40.0);
	// move_forward_inertial(130.0, -50.0, 127.0, 110.0);

	// // push triballs front of the net
	// move_backward(60.0, -40.0, 40.0);
	// turn_left_to_look_at(-110);
	// move_forward_inertial(170.0, -50.0, 80.0, 70.0);
	// turn_right_to_look_at(-10);

	// wings.set_value(1);	
	// left_motors.move(80);
	// right_motors.move(127);
	// pros::delay(2000);
	// left_motors.move(0);
	// right_motors.move(0);
	// // move_forward_inertial(175.0, -50.0, 127.0, 110.0);
	// wings.set_value(0);
	// move_backward(50.0, -50.0, 80.0);
	
	// // push triballs left side front of net
	// turn_left_to_look_at(-45);
	// wings.set_value(1);
	// left_motors.move(100);
	// right_motors.move(80);
	// pros::delay(2300);
	// left_motors.move(0);
	// right_motors.move(0);
	// wings.set_value(0);
	// move_backward(50.0, -80.0, 80.0);
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
