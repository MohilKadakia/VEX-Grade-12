#include "devices.hh"
#include "controls.h"

double move_multiple(double x) {
	return -0.4 * cos((360.0/254.0)*x)+0.6;
}

void drive_robot() {
	while (true) {
		double multiple = 1.0;

		if(master.get_digital(master_UP)) {
			multiple = (double)(std::clamp((int)(multiple*100) +  25, 25, 100))/100;
		} else if (master.get_digital(master_DOWN)) {
			multiple = (double)(std::clamp((int)(multiple*100) - 25, 25, 100))/100;
		}

		int left_y = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		int right_x = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
		
        int left_motor_velocity = left_y + (right_x * move_multiple(right_x)) * multiple;
		int right_motor_velocity = left_y - (right_x * move_multiple(right_x)) * multiple;

		left_motors.move(std::clamp(left_motor_velocity, -127, 127));
		right_motors.move(std::clamp(right_motor_velocity, -127, 127));
		
        pros::delay(10);
    }	
}