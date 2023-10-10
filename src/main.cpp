#include "main.h"
#include "math.h"
#include <string>
#include "ARMS/api.h"
#include "ARMS/config.h"
#include "ARMS/pid.h"



pros::Controller controller (pros::E_CONTROLLER_MASTER);
pros::Motor LM1 (1, pros::E_MOTOR_GEAR_GREEN, false);
pros::Motor LM2 (2, pros::E_MOTOR_GEAR_GREEN, false);
pros::Motor RM1 (3, pros::E_MOTOR_GEAR_GREEN, true);
pros::Motor RM2 (4, pros::E_MOTOR_GEAR_GREEN, true);
double previousError = 0;
double intergal = 0;
pros::Motor_Group LMG({LM1, LM2});
pros::Motor_Group RMG({RM1, RM2});
void initialize() {
	pros::lcd::initialize();
	// arms::init();
	arms::pid::init(0,0,0,0,0,0,0,0,0);
}

void disabled() {}

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


void opcontrol() {
	while (true) {
		double targetD = 2080;
		double currentD = LM1.get_position();
		double error = targetD-currentD;
		double output = pid(error, &previousError, &intergal, 0.25, 0.002, 0.1);
		pros::lcd::set_text(0, std::to_string(output) + " " + std::to_string(currentD));
		LM1.move_velocity(output);
		// pros::lcd::set_text(0, std::to_string(conRX) + " " + std::to_string(conLY) + " ");// + std::to_string(v) +  std::to_string(v2));
		pros::delay(10);
    }	
}
