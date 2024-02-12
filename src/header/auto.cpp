#include "devices.hh"
#include "functions.hh"
#include "auto.hh"
#include <bits/stdc++.h>
#include <iostream>
#include <cmath>
#include "accel.hh"

using namespace std;

double integral_turn = 0;
double previous_error_turn = 0;

double change_numbers(double value) {
    if (std::round(value) < 0){
        return value+360;
    }
    return value;
}

double get_avg_motor_position() {
    // Returns average motor position of robot in degrees
    
    double avg_motor_rotation_left = 0;
    double avg_motor_rotation_right = 0;

    // avg_motor_rotation_left = left_motor_1.get_position();
    // master.set_text(1, 0, std::to_string(avg_motor_rotation_left));

    // master.set_text(2, 0, std::to_string(left_motor_2.get_position()));

    // master.set_text(3, 0, std::to_string(left_motor_3.get_position()));

    // master.set_text(0, 0, std::to_string(right_motor_1.get_position()));

    // master.set_text(1, 0, std::to_string(right_motor_2.get_position()));

    // master.set_text(2, 0, std::to_string(right_motor_3.get_position()));


    for(int i = 0; i < 3; i++) {
        avg_motor_rotation_left += left_motors.get_positions()[i];
        avg_motor_rotation_right += right_motors.get_positions()[i];
    }

    avg_motor_rotation_left /= 3;
    avg_motor_rotation_right /= 3;
    
    double avg_rotation = (avg_motor_rotation_left + avg_motor_rotation_right) / 2;
    master.set_text(0, 0, std::to_string(avg_rotation));
    return avg_rotation;
}

double get_distance_in_degrees(double distance_cm) {
    double wheel_circumfrence = 2 * 3.14159265 * 4; 
    double distance_degrees = (360 * distance_cm) / wheel_circumfrence;

    return distance_degrees;
}

void turn_right_to_look_at(double degrees) {
    double average_inertial_degree = IMU.get_yaw();
    master.clear();
    while (fabs(degrees - average_inertial_degree) > 1) {
        average_inertial_degree = IMU.get_yaw();
        pros::lcd::set_text(0, std::to_string(IMU.get_yaw()));
        pros::lcd::set_text(1, "Turning");
        double output = pid(degrees - average_inertial_degree, &previous_error_turn, &integral_turn, 2.0, 0.1, 0.05);

        left_motors.move_velocity(fabs((std::clamp(output, -90.0, 90.0))));
        right_motors.move_velocity(-fabs(std::clamp(output, -90.0, 90.0)));
        pros::delay(20);
    }
    left_motors.move_velocity(0);
	right_motors.move_velocity(0);
    pros::lcd::set_text(1, "Completed Turn");
}

void turn_right_to_look_at_TEST(double degrees) {
    double average_inertial_degree = IMU.get_yaw();
    master.clear();

    while (std::abs(degrees - average_inertial_degree) > 1.0) {
        average_inertial_degree = IMU.get_yaw();

        double angle_difference = degrees - average_inertial_degree;

        // Adjust for angle wrap-around near 180 degrees
        if (angle_difference > 180.0) {
            angle_difference -= 360.0;
        } else if (angle_difference < -180.0) {
            angle_difference += 360.0;
        }

        pros::lcd::set_text(0, std::to_string(average_inertial_degree));

        double output = pid(angle_difference, &previous_error_turn, &integral_turn, 2.0, 0.1, 0.05);

        left_motors.move_velocity(fabs(std::clamp(output, -90.0, 90.0)));
        right_motors.move_velocity(-fabs(std::clamp(output, -90.0, 90.0)));

        pros::delay(20);
    }

    left_motors.move_velocity(0);
    right_motors.move_velocity(0);
}

void turn_left_to_look_at(double degrees) {
    double average_inertial_degree = IMU.get_yaw();
    double left_motor_velocity = 0;
    double right_motor_velocity = 0;
    master.clear();
    while (fabs(degrees - average_inertial_degree) > 1) {
        average_inertial_degree = IMU.get_yaw();
        pros::lcd::set_text(0, std::to_string(IMU.get_yaw()));
        pros::lcd::set_text(1, "Turning");
        double output = pid(degrees - average_inertial_degree, &previous_error_turn, &integral_turn, 2.0, 0.1, 0.05);

        left_motors.move_velocity(-fabs((std::clamp(output, -90.0, 90.0))));
        right_motors.move_velocity(fabs(std::clamp(output, -90.0, 90.0)));
        pros::delay(20);
    }
    left_motors.move_velocity(0);
	right_motors.move_velocity(0);
    pros::lcd::set_text(1, "Completed Turn for stupid left turn test");
    // pros:delay(10);
}

void turn_left_to_look_at_TEST(double degrees) {
    double inertial_degree = IMU.get_yaw() < 0 ? IMU.get_yaw()+360 : IMU.get_yaw();
    master.clear();

    while (std::abs(degrees - inertial_degree) > 1.0) {
        double inertial_degree = IMU.get_yaw() < 0 ? IMU.get_yaw()+360 : IMU.get_yaw();
        double angle_difference = degrees - inertial_degree;


        pros::lcd::set_text(0, std::to_string(inertial_degree));
        pros::lcd::set_text(1, "Turning");
        double output = pid(angle_difference, &previous_error_turn, &integral_turn, 2.0, 0.1, 0.05);

        left_motors.move_velocity(-fabs(std::clamp(output, -90.0, 90.0)));
        right_motors.move_velocity(fabs(std::clamp(output, -90.0, 90.0)));

        pros::delay(20);
    }

    left_motors.move_velocity(0);
    right_motors.move_velocity(0);
    pros::lcd::set_text(1, "Completed Turn for 180 deg test");
}

void move_backward(double distance_cm, double lowest_speed, double fastest_speed) { 
    double distance_degrees = get_distance_in_degrees(distance_cm);

    double avg_start_rotation = get_avg_motor_position();
    double avg_current_rotation = avg_start_rotation;

    // pid move forward
    while(avg_current_rotation > avg_start_rotation - distance_degrees) {
        avg_current_rotation = get_avg_motor_position();

        double error = (avg_start_rotation - distance_degrees) - avg_current_rotation;
        double output = pid(error, &previous_error_turn, &integral_turn, 2.0, 0.1, 0.05);

        left_motors.move((std::clamp(output, lowest_speed, fastest_speed)));
        right_motors.move((std::clamp(output, lowest_speed, fastest_speed)));
    }
    left_motors.move(0);
    right_motors.move(0);
}

void move_forward(double distance_cm, double lowest_speed, double fastest_speed) {
    // converts cm to degrees to be compared with motor positions (in degrees) 
    double distance_degrees = get_distance_in_degrees(distance_cm);

    // the start and current position is the same
    double avg_start_rotation = get_avg_motor_position();
    double avg_current_rotation = avg_start_rotation;

    // pid move forward
    while(avg_current_rotation <= avg_start_rotation + distance_degrees) {
        avg_current_rotation = get_avg_motor_position();

        double error = (avg_start_rotation + distance_degrees) - avg_current_rotation;
        double output = pid(error, &previous_error_turn, &integral_turn, 2.0, 0.1, 0.05);

        left_motors.move(std::clamp(output, lowest_speed, fastest_speed));
        right_motors.move(std::clamp(output, lowest_speed, fastest_speed));
    }
    left_motors.move(0);
    right_motors.move(0);
}

void move_forward_inertial(double distance_cm, double lowest_base_speed, double fastest_base_speed, double base_speed) {
    // converts cm to degrees to be compared with motor positions (in degrees) 
    double distance_degrees = get_distance_in_degrees(distance_cm);

    // calculates base angle
    double average_inertial_start_angle = IMU.get_yaw();

    // the start and current position is the same
    double avg_start_rotation = get_avg_motor_position();
    double avg_current_rotation = avg_start_rotation;
    // master.set_text(0, 0, std::to_string(avg_current_rotation));
    pros::delay(2000);
    while(avg_current_rotation <= avg_start_rotation + distance_degrees) {
        avg_current_rotation =  get_avg_motor_position();
        master.set_text(0, 0, std::to_string(avg_current_rotation));
        double current_inertial_angle = IMU.get_yaw(); 
        double error = average_inertial_start_angle - current_inertial_angle;
        double output = pid(error, &previous_error_turn, &integral_turn, 7, 0, 0.05);

        left_motors.move(std::clamp(base_speed + output, lowest_base_speed, fastest_base_speed));
        right_motors.move(std::clamp(base_speed - output, lowest_base_speed, fastest_base_speed));
    }
    left_motors.move(0);
    right_motors.move(0);
    left_motors.brake();
    right_motors.brake();
}

void move_forward_inertial_pid(double distance_cm, double lowest_base_speed, double fastest_base_speed) {
    // converts cm to degrees to be compared with motor positions (in degrees) 
    double distance_degrees = get_distance_in_degrees(distance_cm);

    // calculates base angle
    double average_inertial_start_angle = IMU.get_yaw();

    // the start and current position is the same
    double avg_start_rotation = get_avg_motor_position();
    double avg_current_rotation = avg_start_rotation;

    while(avg_current_rotation <= avg_start_rotation + distance_degrees) {
        avg_current_rotation =  get_avg_motor_position();

        double current_inertial_angle = IMU.get_yaw(); 
        double inertial_error = average_inertial_start_angle - current_inertial_angle;
        double inertial_speed_output = pid(inertial_error, &previous_error_turn, &integral_turn, 7, 0, 0.05);

        double distance_error = (avg_start_rotation + distance_degrees) - avg_current_rotation;
        double distance_speed_output = pid(distance_error, &previous_error_turn, &integral_turn, 2.0, 0.1, 0.05);

        left_motors.move(std::clamp(distance_speed_output + inertial_speed_output, lowest_base_speed, fastest_base_speed));
        right_motors.move(std::clamp(distance_speed_output - inertial_speed_output, lowest_base_speed, fastest_base_speed));
    }
    left_motors.move(0);
    right_motors.move(0);
    left_motors.brake();
    right_motors.brake();
}

void move_backward_inertial_pid(double distance_cm, double lowest_base_speed, double fastest_base_speed) {
    // converts cm to degrees to be compared with motor positions (in degrees) 
    double distance_degrees = get_distance_in_degrees(distance_cm);

    // calculates base angle
    double average_inertial_start_angle = IMU.get_yaw();

    // the start and current position is the same
    double avg_start_rotation = get_avg_motor_position();
    double avg_current_rotation = avg_start_rotation;

    while(avg_current_rotation > avg_start_rotation - distance_degrees) {
        avg_current_rotation =  get_avg_motor_position();

        double current_inertial_angle = IMU.get_yaw(); 
        double inertial_error = average_inertial_start_angle - current_inertial_angle;
        double inertial_speed_output = pid(inertial_error, &previous_error_turn, &integral_turn, 7, 0, 0.05);

        double distance_error = (avg_start_rotation - distance_degrees) - avg_current_rotation;
        double distance_speed_output = pid(distance_error, &previous_error_turn, &integral_turn, 2.0, 0.1, 0.05);

        left_motors.move(std::clamp(distance_speed_output - inertial_speed_output, lowest_base_speed, fastest_base_speed));
        right_motors.move(std::clamp(distance_speed_output + inertial_speed_output, lowest_base_speed, fastest_base_speed));
    }
    left_motors.move(0);
    right_motors.move(0);
}

void move_forward_inertial_accel(double distance_cm, double lowest_base_speed, double fastest_base_speed, double base_speed) {
    // converts cm to degrees to be compared with motor positions (in degrees) 
    double distance_meters = distance_cm / 100;

    // calculates base angle
    double average_inertial_start_angle = IMU.get_yaw();

    // the start and current position is the same
    double avg_start_position = position[1];
    double avg_current_position = avg_start_position;

    while(avg_current_position <= avg_start_position + distance_meters) {
        avg_current_position = position[1];

        double current_inertial_angle = IMU.get_yaw(); 
        double error = average_inertial_start_angle - current_inertial_angle;
        double output = pid(error, &previous_error_turn, &integral_turn, 7, 0, 0.05);

        left_motors.move(std::clamp(base_speed + output, lowest_base_speed, fastest_base_speed));
        right_motors.move(std::clamp(base_speed - output, lowest_base_speed, fastest_base_speed));
    }
    left_motors.move(0);
    right_motors.move(0);
}
