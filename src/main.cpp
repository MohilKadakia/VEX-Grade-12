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
	left_motors.move(-78);
	right_motors.move(-120);
	pros::delay(800);
	left_motors.move(0);
	right_motors.move(0);

	pros::delay(100);
	left_motors.move(45);
	right_motors.move(120);
	pros::delay(400);
	wings.set_value(1);
	pros::delay(500);
	wings.set_value(0);
	left_motors.move(127);
	right_motors.move(50);
	pros::delay(750);
	left_motors.move(0);
	right_motors.move(0);
	pros::delay(100);
	left_motors.move(-33);
	right_motors.move(-33);
	pros::delay(300);
	left_motors.move(0);
	right_motors.move(0);
	reset_inertial();
	turn_left_to_look_at(-85);
	double average_inertial_start_angle = (IMU[0].get_yaw() + IMU[1].get_yaw())/2; 

	while(ultrasonic.get_value() < 40 || ultrasonic.get_value() > 70) {
        double current_inertial_angle = (IMU[0].get_yaw() + IMU[1].get_yaw())/2; 
        double error = average_inertial_start_angle - current_inertial_angle;
        double output = pid(error, &previous_error_turn, &integral_turn, 7, 0, 0.05);

        left_motors.move(std::clamp(20 + output, -20.0, 25.0));
        right_motors.move(std::clamp(20 - output, -20.0, 25.0));
    }
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
