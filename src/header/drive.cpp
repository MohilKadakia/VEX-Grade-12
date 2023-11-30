#include "devices.hh"
#include "controls.h"

double move_multiple(double x) {
	return -0.4 * cos((360.0/254.0)*x)+0.6;
}

void drive_robot() {
	double multiple = 1.0;
	while (true) {
		if(master.get_digital(master_UP)) {
			multiple = 1;
		} else if (master.get_digital(master_DOWN)) {
			multiple = -1;
		}

		int left_y = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) * multiple;
		int right_x = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X) * -1;
		
        int left_motor_velocity = (left_y + (right_x * move_multiple(right_x)));
		int right_motor_velocity = (left_y - (right_x * move_multiple(right_x)));

		left_motors.move(std::clamp(left_motor_velocity, -127, 127));
		right_motors.move(std::clamp(right_motor_velocity, -127, 127));
		
        pros::delay(10);
    }	
}