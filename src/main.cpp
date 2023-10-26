#include "main.h"
#include "ARMS/api.h"
#include "ARMS/config.h"
#include <string>
#include <cmath>

pros::Controller master(pros::E_CONTROLLER_MASTER);

// catapult motors
pros::Motor catapult_motor(3, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES);

// // intake motors
// pros::Motor intake_motor_1(8, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);

bool catapult_shooting = false;
bool wings_active = false;
double previous_error = 0;
double integral = 0;
double target_distance = 0;
pros::Motor left_motor_1(1, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor left_motor_2(2, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor left_motor_3(3, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor_Group left_motors({left_motor_1, left_motor_2, left_motor_3});

pros::Motor right_motor_1(4, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_motor_2(7, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_motor_3(6, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor_Group right_motors({right_motor_1, right_motor_2, right_motor_3});

pros::ADIDigitalOut wings('h');

double pid(double error, double* pe, double* in, double kp, double ki, double kd) {
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

void initialize() {
	arms::init();
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Inititalizing v1");
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void fire_catapult_toggle() {
    while (true) {
        pros::lcd::set_text(3, std::to_string(catapult_motor.get_position()));

        if (catapult_shooting) {
            target_distance += 1000;
            pros::lcd::set_text(2, "Rewinding Catapult");
            while (true) {
                double current_distance = catapult_motor.get_position();
                
                if (current_distance > (target_distance - 5)) {
                    break;
                }
                
                double error = target_distance - current_distance;
                double output = pid(error, &previous_error, &integral, 0.25, 0.002, 0.1);
                pros::lcd::set_text(0, std::to_string(output) + " " + std::to_string(current_distance));
                catapult_motor.move_velocity(output);
                pros::delay(10);

                if (!catapult_shooting) {
                    break;
                }
            }       
        }

        pros::delay(10);
    }
}

double wings_pneumatic() {
    while(true) {
        if (wings_active) {
            pros::lcd::set_text(2, "Pneumatics Active");
            while (wings_active) {
                wings.set_value(true);
            }       
        }
    }
    pros::delay(10);
}

double move_multiple(double x) {
	return -0.4 * cos((360.0/254.0)*x)+0.6;
}

std::string set_controller_temperatures(pros::Motor_Group *motors) {
	std::string temperatures = "";
	for(int i = 0; i < 3; i++) {
		temperatures += motors->get_temperatures()[i];
	}
	return temperatures;
}

void drive_robot() {
	while (true) {
		int right_x = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
		double multiple = 1.0;
		int moveL = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) + (right_x * move_multiple(right_x)) * multiple;
		int moveR = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) - (right_x * move_multiple(right_x)) * multiple;
		left_motors.move(std::clamp(moveL, -127, 127));
		right_motors.move(std::clamp(moveR, -127, 127));
		pros::lcd::set_text(1, "Left: " + std::to_string(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)));
		pros::lcd::set_text(2, "Right: " + std::to_string(master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y)));
		pros::delay(10);
    }	
}

void opcontrol() {
	pros::lcd::set_text(1, "Enters the OPControl");
	pros::Controller master(pros::E_CONTROLLER_MASTER);
    pros::Task catapult_toggle_task(fire_catapult_toggle);
    pros::Task wings_pneumatic_task(wings_pneumatic);
    pros::Task drive_task(drive_robot);
	master.clear();

	while (true) {
		// master.set_text(2, 0, set_controller_temperatures(&left_motors));
		master.set_text(2, 0, std::to_string(left_motor_1.get_temperature()));
		master.set_text(1, 0, set_controller_temperatures(&right_motors));

		// Check R2 button to toggle firing
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            catapult_shooting = !catapult_shooting;
            while (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
                pros::delay(10);
            }
        }

		// Check A button to toggle firing
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
            wings_active = !wings_active;
            while (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
                pros::delay(10);
            }
        }
    }	
}