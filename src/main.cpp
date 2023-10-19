#include "main.h"
#include "ARMS/api.h"
#include "ARMS/config.h"
#include <string>

bool catapult_shooting = false;
double previous_error = 0;
double integral = 0;
double target_distance = 0;

pros::Controller master(pros::E_CONTROLLER_MASTER);

pros::Motor left_motor_1(1, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor left_motor_2(2, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor left_motor_3(3, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor_Group left_motors({left_motor_1, left_motor_2, left_motor_3});

pros::Motor right_motor_1(4, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_motor_2(7, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_motor_3(6, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor_Group right_motors({right_motor_1, right_motor_2, right_motor_3});
pros::Motor_Group all_motors({left_motor_1, left_motor_2, left_motor_3, right_motor_1, right_motor_2, right_motor_3});

// catapult motors
pros::Motor catapult_motor(3, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES);

// Assuming the inertial sensor is connected to port 10
pros::Imu inertial_sensor(10);

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
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
    // target_distance = 1000;
    // while (true) {
    //     double current_distance = catapult_motor.get_position();
        
    //     if (current_distance > (target_distance - 5)) {
    //         break;
    //     }
        
    //     double error = target_distance - current_distance;
    //     double output = pid(error, &previous_error, &integral, 0.25, 0.002, 0.1);
    //     pros::lcd::set_text(0, std::to_string(output) + " " + std::to_string(current_distance));
    //     all_motors.move_velocity(output);
    //     pros::delay(10);
    // }       
}

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

        pros::lcd::set_text(3, std::to_string(inertial_sensor.get_yaw()));
        pros::delay(10);

        

        // if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
        //         // Set the desired turning angle in degrees
        //         inertial_sensor.reset();
        //         const double target_angle = 90.0;

        //         // Set the motor power for turning
        //         const int motor_power = 50; // Adjust this value as needed

        //         // Calculate the starting angle
        //         double start_angle = inertial_sensor.get_yaw();

        //         // Calculate the angle to turn by
        //         double angle_to_turn = target_angle - start_angle;

        //         // Determine the direction of the turn (left or right)
        //         int direction = (angle_to_turn > 0) ? 1 : -1;

        //         // While the absolute error is greater than a threshold, keep turning
        //         while (fabs(angle_to_turn) > 1.0) {
        //             // Read the current angle from the inertial sensor
        //             double current_angle = inertial_sensor.get_yaw();
        //             pros::lcd::set_text(1, ("Current Angle:" + std::to_string(current_angle)));

        //             // Calculate the remaining angle to turn
        //             angle_to_turn = target_angle - current_angle;
        //             pros::lcd::set_text(2, ("Remaining Angle to Turn:" + std::to_string(angle_to_turn)));

        //             // Adjust motor power based on the remaining angle
        //             int power = direction * motor_power;

        //             // Apply power to the left and right drive motors
        //             // Modify this according to your robot's motor configuration

        //             left_motors.move(power);
        //             right_motors.move(-power);

        //             // You may also add a delay to control the turn speed
        //             pros::delay(10); // Adjust the delay as needed
        //         }

        //         // Stop the motors when the turn is complete
        //         all_motors.move(0);
        // }

        // if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
        //         // Set the desired turning angle in degrees
        //         const double target_angle = -90.0;

        //         // Set the motor power for turning
        //         const int motor_power = 50; // Adjust this value as needed

        //         // Calculate the starting angle
        //         double start_angle = inertial_sensor.get_rotation();

        //         // Calculate the angle to turn by
        //         double angle_to_turn = target_angle - start_angle;

        //         // Determine the direction of the turn (left or right)
        //         int direction = (angle_to_turn > 0) ? 1 : -1;

        //         // While the absolute error is greater than a threshold, keep turning
        //         while (fabs(angle_to_turn) > 1.0) {
        //             // Read the current angle from the inertial sensor
        //             double current_angle = inertial_sensor.get_rotation();
        //             pros::lcd::set_text(1, ("Current Angle:" + std::to_string(current_angle)));

        //             // Calculate the remaining angle to turn
        //             angle_to_turn = target_angle - current_angle;
        //             pros::lcd::set_text(2, ("Remaining Angle to Turn:" + std::to_string(angle_to_turn)));

        //             // Adjust motor power based on the remaining angle
        //             int power = direction * motor_power;

        //             // Apply power to the left and right drive motors
        //             // Modify this according to your robot's motor configuration

        //             left_motors.move(power);
        //             right_motors.move(-power);

        //             // You may also add a delay to control the turn speed
        //             pros::delay(10); // Adjust the delay as needed
        //         }

        //         // Stop the motors when the turn is complete
        //         all_motors.move(0);
        // }

        // if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
        //         // Set the desired turning angle in degrees
        //         const double target_angle = 45.0;

        //         // Set the motor power for turning
        //         const int motor_power = 50; // Adjust this value as needed

        //         // Calculate the starting angle
        //         double start_angle = inertial_sensor.get_rotation();

        //         // Calculate the angle to turn by
        //         double angle_to_turn = target_angle - start_angle;

        //         // Determine the direction of the turn (left or right)
        //         int direction = (angle_to_turn > 0) ? 1 : -1;

        //         // While the absolute error is greater than a threshold, keep turning
        //         while (fabs(angle_to_turn) > 1.0) {
        //             // Read the current angle from the inertial sensor
        //             double current_angle = inertial_sensor.get_rotation();
        //             pros::lcd::set_text(1, ("Current Angle:" + std::to_string(current_angle)));

        //             // Calculate the remaining angle to turn
        //             angle_to_turn = target_angle - current_angle;
        //             pros::lcd::set_text(2, ("Remaining Angle to Turn:" + std::to_string(angle_to_turn)));

        //             // Adjust motor power based on the remaining angle
        //             int power = direction * motor_power;

        //             // Apply power to the left and right drive motors
        //             // Modify this according to your robot's motor configuration

        //             left_motors.move(power);
        //             right_motors.move(-power);

        //             // You may also add a delay to control the turn speed
        //             pros::delay(10); // Adjust the delay as needed
        //         }

        //         // Stop the motors when the turn is complete
        //         all_motors.move(0);
        // }
    }
}