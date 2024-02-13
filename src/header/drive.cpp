#include "devices.hh"
#include "controls.h"
#include "math.h"
double* convert(double inputX, double inputY){
	double t = atan2(inputY, inputX);
	double outputX = cos(t);
	double outputY = sin(t)*signbit(inputX*inputY);
	double outputs[2] = {outputX, outputY};
	return outputs;	
}

void drive_robot() {
	int left_y = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
	int right_x = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X) * -1;
	double* outputs = convert(right_x, left_y);
	int left_motor_velocity = outputs[1] - outputs[0];
	int right_motor_velocity = outputs[1] + outputs[0];

	left_motors.move(left_motor_velocity);
	right_motors.move(right_motor_velocity);
}