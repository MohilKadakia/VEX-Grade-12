#include "main.h"
#include "math.h"
#include <string>
#include <iostream>
#include <fstream>
#include "ARMS/api.h"
#include "ARMS/config.h"
#include "ARMS/pid.h"
#define endl '\n'


pros::Controller controller (pros::E_CONTROLLER_MASTER);

pros::Motor left_motor_1(1, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor left_motor_2(2, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor left_motor_3(3, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor right_motor_1(4, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_motor_2(7, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_motor_3(6, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor_Group left_motors({left_motor_1, left_motor_2, left_motor_3});
pros::Motor_Group right_motors({right_motor_1, right_motor_2, right_motor_3});

double previousErrorL[] = {0, 0, 0};
double intergalL[] = {0, 0, 0};
double previousErrorR[] = {0, 0, 0};
double intergalR[] = {0, 0, 0};

std::ofstream XValues("xvalues.txt");
std::ofstream YValues("yvalues.txt");

void initialize() {
	pros::lcd::initialize();
	// arms::init();
	arms::pid::init(0,0,0,0,0,0,0,0,0);
	XValues << "test";
}

void disabled() {XValues.close();}

void competition_initialize() {}

void autonomous() {}

double pid(double error, double* pe, double* in, double kp, double ki,
           double kd) {

	double derivative = error - *pe;
	if ((*pe > 0 && error < 0) || (*pe < 0 && error > 0))
		*in = 0; // remove integral at zero error
	double speed = error * kp + *in * ki + derivative * kd;

	// only let integral wind up if near the target
	if (fabs(error) < 15) {
		*in += error;
	}

	*pe = error;

	return speed;
}

double pidTurn(double angle, pros::motor_gearset_e_t gearset) {
	/*
	Convert angle to encoder units
	Runs PID function

	BLUE  (6:1)  : 300  Encoder Units / 360 Degrees? (300/rev)
	GREEN (18:1) : 900  Encoder Units / 360 Degrees  (900/rev)
	RED   (36:1) : 1800 Envoder Units / 360 Degrees? (1800/rev)
	*/

	double units = 0;

	switch(gearset) {
		case 0: units = angle*0.2; break; // E_MOTOR_GEARSET_36
		case 1: units = angle*0.4; break; // E_MOTOR_GEARSET_18
		case 2: units = angle*1.2; break; // E_MOTOR_GEARSET_06
	}
	
	return 0; // Add pid() function
}

void opcontrol() {
	int i = 0;
	while (true) {
		double currentAngleL[3] = {left_motor_1.get_position(), left_motor_2.get_position(), left_motor_3.get_position()};
		double currentAngleR[3] = {right_motor_1.get_position(), right_motor_2.get_position(), right_motor_3.get_position()};
		double targetAngleL = 900;
		double targetAngleR = 900;

		double errorL[3];
		double errorR[3];

		double outputL[3];
		double outputR[3];

		for(int i = 0; i < 3; i++) {
			currentAngleL[i] = left_motors.get_positions()[i];
			currentAngleR[i] = right_motors.get_positions()[i];

			errorL[i] = targetAngleL-currentAngleL[i];
			errorR[i] = targetAngleR-currentAngleR[i];

			outputL[i] = pid(errorL[i], &previousErrorL[i], &intergalL[i], 0.8, 0.0005, 0.4);
			outputR[i] = pid(errorR[i], &previousErrorR[i], &intergalR[i], 0.8, 0.0005, 0.4);

		}
		
		pros::lcd::set_text(0, "Left: " + std::to_string(outputL[0]) + " " + std::to_string(left_motor_1.get_position()));
		pros::lcd::set_text(1, std::to_string(i));
		// std::cout<<outputL1<<endl<<outputR1<<endl;
		// pros::lcd::set_text(1, "Right: " + std::to_string(outputR) + " " + std::to_string(currentAngleR));
		for(int i = 0; i < 3; i++) {
			left_motors[i].move_velocity(outputL[i]);
			right_motors[i].move_velocity(outputR[i]);
		}

		pros::delay(10);
    }	
}
