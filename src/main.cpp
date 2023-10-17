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
pros::Motor LM1 (1, pros::E_MOTOR_GEAR_GREEN, false);
pros::Motor LM2 (2, pros::E_MOTOR_GEAR_GREEN, false);
pros::Motor RM1 (3, pros::E_MOTOR_GEAR_GREEN, true);
pros::Motor RM2 (4, pros::E_MOTOR_GEAR_GREEN, true);
double previousErrorL = 0;
double intergalL = 0;
double previousErrorR = 0;
double intergalR = 0;
std::ofstream XValues("xvalues.txt");
std::ofstream YValues("yvalues.txt");
pros::Motor_Group LMG({LM1, LM2});
pros::Motor_Group RMG({RM1, RM2});
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

double pidTurn(double angle) {
	/*
	Convert angle to encoder units
	Runs PID function

	BLUE  (6:1)  : 300  Encoder Units / 360 Degrees? (300/rev)
	GREEN (18:1) : 900  Encoder Units / 360 Degrees  (900/rev)
	RED   (36:1) : 1800 Envoder Units / 360 Degrees? (1800/rev)
	*/
	return 0;
}

void opcontrol() {
	int i = 0;
	double targetAngleL = 900;
	while (true) {
		// Call PID for each movement motor so it doesn't over/undershoot due to averaging the positions
		double currentAngleL = LM1.get_position();
		if(currentAngleL == targetAngleL && i < 250) {
			targetAngleL += 900;
			i++;
		}
		// double currentAngleL = (LMG.get_positions()[1] + LMG.get_positions()[0])/2;
		double targetAngleR = -10000;
		double currentAngleR = (RMG.get_positions()[1] + RMG.get_positions()[0])/2;
		double errorL = targetAngleL-currentAngleL;
		double errorR = targetAngleR-currentAngleR;
		double outputL = pid(errorL, &previousErrorL, &intergalL, 0.25, 0.003, 0.11);
		double outputR = pid(errorR, &previousErrorR, &intergalR, 0.25, 0.002, 0.1);
		pros::lcd::set_text(0, "Left: " + std::to_string(outputL) + " " + std::to_string(LM1.get_position()));
		pros::lcd::set_text(1, std::to_string(i));
		std::cout<<outputL<<endl<<outputR<<endl;
		// pros::lcd::set_text(1, "Right: " + std::to_string(outputR) + " " + std::to_string(currentAngleR));
		LM1.move_velocity(outputL);
		//RMG.move_velocity(outputR);
		pros::delay(10);
    }	
}
