#include "main.h"
#include "ARMS/api.h"
#include "ARMS/config.h"

#include <string>
#include <cmath>

#include "header/devices.h"
#include "header/pid.hh"
#include "header/ports.h"
#include "header/catapult.hh"
#include "header/wings.hh"

void initialize()
{
	arms::init();
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Inititalizing v1");
	reset_inertial();
}

void disabled() 
{
	pros::lcd::clear();
	while (1){
		pros::lcd::set_text(0, "Disabled");
		pros::delay(10);
	}
}

void competition_initialize()
{
	reset_inertial();
}

void autonomous()
{
	pros::lcd::clear();
	reset_inertial();
	while (1){
		pros::lcd::set_text(0, "Auto");
		pros::delay(10);
	}
}

void reset_inertial()
{
    IMU[0].reset();
    IMU[1].reset();
}

double move_multiple(double x) {
	return -0.4 * cos((360.0/254.0)*x)+0.6;
}

void drive_robot() {
	while (true) {
		double multiple = 1.0;
		int left_y = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		int right_x = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
		
        int left_motor_velocity = left_y + (right_x * move_multiple(right_x)) * multiple;
		int right_motor_velocity = left_y - (right_x * move_multiple(right_x)) * multiple;

		left_motors.move(std::clamp(left_motor_velocity, -127, 127));
		right_motors.move(std::clamp(right_motor_velocity, -127, 127));
		
        pros::delay(10);
    }	
}

void opcontrol() {
    pros::lcd::clear();
	reset_inertial();
    
    pros::Task wings_pneumatic_task(wings_pneumatic);
    pros::Task drive_task(drive_robot);
	pros::Task catapult_task(catapult_trigger);
	pros::Task debug_task(debug_values);

	while (true) {
        if (catapult_active) {
                catapult_motor.move_absolute(1000.0, 600);
        }

		// Check A button to toggle firing
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
            wings_active = !wings_active;
            while (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
                pros::delay(10);
            }
        }

        pros::delay(10);
    }	
}