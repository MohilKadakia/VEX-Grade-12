#include "main.h"
#include "ARMS/api.h"
#include "ARMS/config.h"
#include <string>

bool catapult_shooting = false;
double previous_error = 0;
double integral = 0;
double target_distance = 0;

bool catapult_single_shoot = false;

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

// double previousErrorL[] = {0, 0, 0};
// double intergalL[] = {0, 0, 0};
// double previousErrorR[] = {0, 0, 0};
// double intergalR[] = {0, 0, 0};

double previousErrorL = 0;
double intergalL = 0;
double previousErrorR = 0;
double intergalR = 0;


// catapult motors
pros::Motor catapult_motor(3, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES);

// Assuming the inertial sensor is connected to port 10
pros::Imu IMU[] = {9, 10};

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
    for (int i=0;i < 2; i++) {
        IMU[i].reset();
    }
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

        if (catapult_single_shoot) {
            target_distance += 1000;
            pros::lcd::set_text(2, "Rewinding Single Shoot Catapult");
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
            }
            catapult_single_shoot = !catapult_single_shoot;       
        }

        pros::delay(10);
    }
}

void fire_catapult_single() {
    while (true) {
        pros::lcd::set_text(3, std::to_string(catapult_motor.get_position()));

        if (catapult_single_shoot) {
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


void turn_right(double target_angle) {
    while (true) {
        double error_inertial_1 = target_angle - IMU[0].get_yaw();
        double error_inertial_2 = target_angle - IMU[1].get_yaw();

        double average_inertial_error = (error_inertial_1 + error_inertial_2) / 2;

        double left_motor_velocity = pid(error_inertial_1, &previousErrorL, &intergalL, 50.0, 0.0005, 0.4);
        double right_motor_velocity = pid(error_inertial_2, &previousErrorR, &intergalR, 50.0, 0.0005, 0.4);

        left_motors.move_velocity(left_motor_velocity);
        right_motors.move_velocity(-right_motor_velocity);

        if (average_inertial_error > 5 || average_inertial_error < 5) {
            break;
        }
    }
}

void opcontrol() {
    pros::Task catapult_toggle_task(fire_catapult_toggle);
    pros::Task catapult_single_shoot_task(fire_catapult_single);    
    pros::Task drive_task(drive_robot);

    while (true) {

        for (int i=0;i < 2; i++) {
            pros::lcd::set_text(i+1, std::to_string(IMU[i].get_yaw()));
        }

        // Press R2 button to toggle firing
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            catapult_shooting = !catapult_shooting;
            while (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
                pros::delay(10);
            }
        }

        // Press A button to shoot once
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
            catapult_single_shoot = !catapult_single_shoot;
            while (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
                pros::delay(10);
            }
        }

        turn_right(90);
                
		// double outputL[3];
		// double outputR[3];

        // double error_inertial_1 = targetAngle - IMU[0].get_yaw();
        // double error_inertial_2 = targetAngle - IMU[1].get_yaw();

        // pros::lcd::set_text(3, std::to_string(error_inertial_1));
        // pros::lcd::set_text(4, std::to_string(error_inertial_2));

        // for(int i = 0; i < 3; i++) {
		// 	outputL[i] = pid(error_inertial_1, &previousErrorL[i], &intergalL[i], 50.0, 0.0005, 0.4);
		// 	outputR[i] = pid(error_inertial_2, &previousErrorR[i], &intergalR[i], 50.0, 0.0005, 0.4);
		// 	left_motors[i].move_velocity(outputL[i]);
		// 	right_motors[i].move_velocity(-outputR[i]);
        //     pros::lcd::set_text(5, std::to_string(outputL[i]));
        //     pros::lcd::set_text(6, std::to_string(outputR[i]));            
		// }
		
        pros::delay(10);

    }
}