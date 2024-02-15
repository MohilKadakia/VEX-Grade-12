#include "devices.hh"
#include "controls.h"
#include "math.h"
double fixInputX(double inputX, double inputY){
	double t = atan2(inputY, inputX);
	return cos(t)*((signbit(inputX)*2)-1);
}
double fixInputY(double inputX, double inputY){
	double t = atan2(inputY, inputX);
	return sin(t)*((signbit(inputX*inputY)*2)-1);
}
int reversed = -1;
double speed = 1;
void drive_robot() {
	int left_y = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
	int right_x = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
	pros::lcd::set_text(0, std::to_string(fixInputY(right_x, left_y)*left_y) + ' ' + std::to_string(fixInputX(right_x, left_y)*right_x));
	int left_motor_velocity = fixInputY(right_x, left_y)*left_y + fixInputX(right_x, left_y)*right_x;
	int right_motor_velocity = fixInputY(right_x, left_y)*left_y - fixInputX(right_x, left_y)*right_x;

	left_motors.move(left_motor_velocity*reversed*speed);
	right_motors.move(right_motor_velocity*reversed*speed);
	
}
void driver_options(){
	while (true){
		if (master.get_digital(master_Y)){
			reversed = reversed == 1 ? -1 : 1;
			while (master.get_digital(master_Y)){
				pros::delay(10);
			}
		}
		if (master.get_digital(master_UP)){
			speed = std::clamp(speed+0.25, 0.25, 1.0);
			while (master.get_digital(master_UP)){
				pros::delay(10);
			}
		}
		if (master.get_digital(master_DOWN)){
			speed = std::clamp(speed-0.25, 0.25, 1.0);
			while (master.get_digital(master_DOWN)){
				pros::delay(10);
			}
		}
		pros::delay(20);
	}
}