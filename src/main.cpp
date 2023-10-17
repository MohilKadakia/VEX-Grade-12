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
double previousErrorL[] = {0, 0};
double intergalL[] = {0, 0};
double previousErrorR[] = {0, 0};
double intergalR[] = {0, 0};

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
		double currentAngleL[] = {LM1.get_position(), LM2.get_position()};
		double targetAngleL[] = {-10000, -10000};

		double currentAngleR[] = {RM1.get_position(), RM2.get_position()};
		double targetAngleR[] = {-10000, -10000};

		double errorL[] = {targetAngleL[0]-currentAngleL[0], targetAngleL[1]-currentAngleL[1]};
		double errorR[] = {targetAngleR[0]-currentAngleR[0], targetAngleR[1]-currentAngleR[1]};

		double outputL1 = pid(errorL[0], &previousErrorL[0], &intergalL[0], 0.25, 0.003, 0.11);
		double outputL2 = pid(errorL[1], &previousErrorL[1], &intergalL[1], 0.25, 0.003, 0.11);
		double outputR1 = pid(errorR[0], &previousErrorR[0], &intergalR[0], 0.25, 0.002, 0.1);
		double outputR2 = pid(errorR[1], &previousErrorR[1], &intergalR[1], 0.25, 0.002, 0.1);
		pros::lcd::set_text(0, "Left: " + std::to_string(outputL1) + " " + std::to_string(LM1.get_position()));
		pros::lcd::set_text(1, std::to_string(i));
		std::cout<<outputL1<<endl<<outputR1<<endl;
		// pros::lcd::set_text(1, "Right: " + std::to_string(outputR) + " " + std::to_string(currentAngleR));
		LM1.move_velocity(outputL1);
		LM2.move_velocity(outputL2);
		RM1.move_velocity(outputR1);
		RM2.move_velocity(outputR2);

		pros::delay(10);
    }	
}
