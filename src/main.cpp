#include "main.h"
#include "ARMS/api.h"
#include "ARMS/config.h"
#include <string>

pros::Controller master(pros::E_CONTROLLER_MASTER);

// catapult motors
pros::Motor catapult_motor(3, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES);

// // intake motors
// pros::Motor intake_motor_1(8, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);

bool catapult_shooting = false;
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

void drive_robot() {
	while (true) {
		int moveL = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) + master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
		int moveR = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) - master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
		left_motors.move(std::clamp(moveL, -127, 127));
		right_motors.move(std::clamp(moveR, -127, 127));
		pros::lcd::set_text(1, "Left: " + std::to_string(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)));
		pros::lcd::set_text(2, "Right: " + std::to_string(master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y)));
		pros::delay(10);
    }	
}


void opcontrol() {
    pros::Task catapult_toggle_task(fire_catapult_toggle);
    pros::Task drive_task(drive_robot);

    while (true) {
        // Check R2 button to toggle firing
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            catapult_shooting = !catapult_shooting;
            while (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
                pros::delay(10);
            }
        }

    }
}
