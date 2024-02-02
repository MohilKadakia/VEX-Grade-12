#include "main.h"

#include <string>
#include <cmath>
// #include "gif-pros/gifclass.hpp"
#include "header/puncher.hh"
#include "header/controls.h"
#include "header/devices.hh"
#include "header/drive.hh"
#include "header/functions.hh"
#include "header/ports.h"
#include "header/wings.hh"
// #include "header/auto.hh"
#include "header/intake.hh"
#include "header/hang.hh"
double velox = 0;
double veloy = 0;
double veloz = 0;
double dispx = 0;
double dispy = 0;
double dispz = 0;
double pt = pros::millis();
void initialize()
{
	IMU.reset(true);
	pros::lcd::initialize();

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
	// left_motors.move(-78);
	// right_motors.move(-120);
	// pros::delay(766);
	// left_motors.move(0);
	// right_motors.move(0);

	// pros::delay(100);
	// left_motors.move(45);
	// right_motors.move(120);
	// pros::delay(400);
	// wings.set_value(1);
	// pros::delay(500);
	// wings.set_value(0);
	// left_motors.move(127);
	// right_motors.move(50);
	// pros::delay(750);
	// left_motors.move(0);
	// right_motors.move(0);
	// pros::delay(100);
	// left_motors.move(-33);
	// right_motors.move(-33);
	// pros::delay(300);
	// left_motors.move(0);
	// right_motors.move(0);
	// reset_inertial();
	// turn_left_to_look_at(-85);
	// double average_inertial_start_angle = IMU.get_yaw();

	// while(ultrasonic.get_value() < 40 || ultrasonic.get_value() > 70) {
    //     double current_inertial_angle = IMU.get_yaw(); 
    //     double error = average_inertial_start_angle - current_inertial_angle;
    //     double output = pid(error, &previous_error_turn, &integral_turn, 7, 0, 0.05);

    //     left_motors.move(std::clamp(20 + output, -20.0, 25.0));
    //     right_motors.move(std::clamp(20 - output, -20.0, 25.0));
    // }
	// left_motors.move(0);
	// right_motors.move(0);
}

void opcontrol() {
	// pros::Task catapult_task(puncher_trigger);
	// pros::Task wings_task(wings_trigger);
	// pros::Task hang_task(hang_trigger);
	while (true) {
		// intake();
		// drive_robot();
		// handle_puncher();
		// handle_wings();
		// handle_hang();
		VeloNDispViaAccel(&velox, &veloy, &veloz, &dispx, &dispy, &dispz, &pt);
		pros::lcd::set_text(1, std::to_string(velox));
		// pros::lcd::set_text(1, std::to_string(IMU.get_accel().x));
		// pros::lcd::set_text(2, std::to_string( (double) pros::millis()) );
		// pros::lcd::set_text(3, std::to_string(pt));
		// pros::lcd::set_text(2, std::to_string(veloz));
		// pros::lcd::set_text(3, std::to_string(dispx));
		// pros::lcd::set_text(4, std::to_string(dispy));
		// pros::lcd::set_text(5, std::to_string(dispz));
		pros::delay(20);
	}        
}
