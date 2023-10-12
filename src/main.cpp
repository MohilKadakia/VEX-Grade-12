#include "main.h"
#include "ARMS/api.h"
#include "ARMS/config.h"
#include <string>

pros::Controller master(pros::E_CONTROLLER_MASTER);

// Left Side Motors
pros::Motor left_motor_1(1, pros::E_MOTOR_GEAR_GREEN, false, pros::E_MOTOR_ENCODER_DEGREES);
// pros::Motor left_motor_2(2, pros::E_MOTOR_GEAR_GREEN, false, pros::E_MOTOR_ENCODER_DEGREES);
// pros::Motor left_motor_3(3, pros::E_MOTOR_GEAR_GREEN, false, pros::E_MOTOR_ENCODER_DEGREES);
// pros::Motor_Group left_motor_group({left_motor_1, left_motor_2, left_motor_3});

// Right Side Motors
pros::Motor right_motor_1(2, pros::E_MOTOR_GEAR_GREEN, true, pros::E_MOTOR_ENCODER_DEGREES);
// pros::Motor right_motor_2(5, pros::E_MOTOR_GEAR_GREEN, true, pros::E_MOTOR_ENCODER_DEGREES);
// pros::Motor right_motor_3(6, pros::E_MOTOR_GEAR_GREEN, true, pros::E_MOTOR_ENCODER_DEGREES);
// pros::Motor_Group right_motor_group({right_motor_1, right_motor_2, right_motor_3});

// Drive Train Motors
// pros::Motor_Group drive_train({left_motor_1, left_motor_2, left_motor_3, right_motor_1, right_motor_2, right_motor_3});

// catapult motors
pros::Motor catapult_motor(3, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES);

// // intake motors
// pros::Motor intake_motor_1(8, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);

bool catapult_shooting = false;
double previous_error = 0;
double integral = 0;

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

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	arms::init();
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Inititalizing v1");
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

void fire_catapult() {
    double target_distance = 0;
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
                // pros::lcd::set_text(0, std::to_string(conRX) + " " + std::to_string(conLY) + " ");// + std::to_string(v) +  std::to_string(v2));
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
		int power = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		int turn = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
		int left = power + turn;
		int right = power + turn;
		left_motor_1.move(left);
		right_motor_1.move(right);

		pros::delay(2);
    }
}


/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
    pros::Task catapult_task(fire_catapult);
    pros::Task drive_task(drive_robot);

    while (true) {
        // Check R2 button to toggle firing
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            catapult_shooting = !catapult_shooting;
            while (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
                pros::delay(10);
            }
        }

        pros::delay(10);
    }
}