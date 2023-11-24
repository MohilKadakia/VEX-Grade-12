#include "devices.hh"
#include "functions.hh"
#include "auto.hh"
#include <bits/stdc++.h>

double integral_turn = 0;
double previous_error_turn = 0;

double change_numbers(double value){
    if (std::round(value) < 0){
        return value+360;
    }
    return value;
}

void turn(double degree) {
    double avgInertialYaw = 0;
    bool buffer_period_active = false;
    master.clear();
    pros::lcd::set_text(0, "Has entered the turn" + std::to_string((IMU[0].get_yaw() + IMU[1].get_yaw()) / 2));

    while (fabs(degree-avgInertialYaw)>1){
        avgInertialYaw = (change_numbers(IMU[0].get_yaw())+change_numbers(IMU[1].get_yaw()))/2;
        // master.set_text(0,0,std::to_string(buffer_period_active) + " " + std::to_string(IMU[0].get_yaw()) + " " + std::to_string(IMU[1].get_yaw()));
        // master.set_text(0,0,"This is currently at turning" + std::to_string((IMU[0].get_yaw() + IMU[1].get_yaw()) / 2));
        pros::lcd::set_text(1, "This is currently turning at" + std::to_string((IMU[0].get_yaw() + IMU[1].get_yaw()) / 2));

        pros::lcd::set_text(0, std::to_string((int) round(avgInertialYaw))+ " " + std::to_string(buffer_period_active));
        pros::lcd::set_text(1, "Turning");
        double output = pid(degree-avgInertialYaw, &previous_error_turn, &integral_turn, 2.0, 0.1, 0.05);
        left_motors.move_velocity(output);
        right_motors.move_velocity(-output);
        pros::delay(20);
    }
    int bufferTime = pros::millis()+3000;
    while(pros::millis() < bufferTime) {
        buffer_period_active = true;
        avgInertialYaw = (change_numbers(IMU[0].get_yaw())+change_numbers(IMU[1].get_yaw()))/2;
        master.set_text(0,0,std::to_string(buffer_period_active) + " " + std::to_string(IMU[0].get_yaw()) + " " + std::to_string(IMU[1].get_yaw()));
        pros::lcd::set_text(0, std::to_string((int) round(avgInertialYaw))+ " " + std::to_string(buffer_period_active));
        double output = pid(degree-avgInertialYaw, &previous_error_turn, &integral_turn, 2.0, 0.1, 0.05);
        left_motors.move_velocity(output);
        right_motors.move_velocity(-output);
        pros::delay(20);
    }
    left_motors.move_velocity(0);
	right_motors.move_velocity(0);
    buffer_period_active = false;
    pros::lcd::set_text(1, "Completed Turn");
}

void move(double cm) {
    double distance = cm*23.7;

    double start_rotation_left = 0;
    double start_rotation_right = 0;

    for(int i = 0; i < 3; i++) {
        start_rotation_left += left_motors.get_positions()[i];
        start_rotation_right += right_motors.get_positions()[i];
    }

    start_rotation_left /= 3;
    start_rotation_right /= 3;

    double avg_left_motors = start_rotation_left;
    double avg_right_motors = start_rotation_right;

    while(fabs(start_rotation_left + distance - avg_left_motors) > 1) {
        avg_left_motors = 0;
        avg_right_motors = 0;
        for(int i = 0; i < 3; i++) {
            avg_left_motors += left_motors.get_positions()[i];
            avg_right_motors += right_motors.get_positions()[i];
        }
        avg_left_motors /= 3;
        avg_right_motors /= 3;
        double output = pid((start_rotation_left+distance)-avg_left_motors, &previous_error_turn, &integral_turn, 2.0, 0.1, 0.05);
        left_motors.move(std::clamp(output, -60.0, 60.0));
        right_motors.move(std::clamp(output, -60.0, 60.0));
    }
}

void turn_right_to_look_at(double degrees) {
    double average_inertial_degree = (IMU[0].get_yaw() + IMU[1].get_yaw())/2;
    master.clear();
    while (fabs(degrees - average_inertial_degree) > 1) {
        average_inertial_degree = (IMU[0].get_yaw() + IMU[1].get_yaw())/2;
        // master.set_text(0,0, std::to_string((IMU[0].get_yaw() + IMU[1].get_yaw()) / 2));
        pros::lcd::set_text(0, std::to_string((IMU[0].get_yaw() + IMU[1].get_yaw()) / 2));
        pros::lcd::set_text(1, "Turning");
        double output = pid(degrees - average_inertial_degree, &previous_error_turn, &integral_turn, 2.0, 0.1, 0.05);
        master.set_text(0,0, std::to_string(output));
        // left_motors.move_velocity(-output);
        // right_motors.move_velocity(output);

        left_motors.move_velocity(-(std::clamp(output, 0.0, 90.0)));
        right_motors.move_velocity((std::clamp(output, 0.0, 90.0)));
        pros::delay(20);
    }
    left_motors.move_velocity(0);
	right_motors.move_velocity(0);
    pros::lcd::set_text(1, "Completed Turn");
    // master.set_text(0,0, ("Final deg:" + std::to_string((IMU[0].get_yaw() + IMU[1].get_yaw()) / 2)));
}

void turn_left_to_look_at(double degrees)
{
    double average_inertial_degree = (IMU[0].get_yaw() + IMU[1].get_yaw())/2;
    master.clear();
    while (fabs(degrees - average_inertial_degree) > 1) {
        average_inertial_degree = (IMU[0].get_yaw() + IMU[1].get_yaw())/2;
        master.set_text(0,0, std::to_string((IMU[0].get_yaw() + IMU[1].get_yaw()) / 2));
        pros::lcd::set_text(0, std::to_string((IMU[0].get_yaw() + IMU[1].get_yaw()) / 2));
        pros::lcd::set_text(1, "Turning");
        double output = pid(degrees - average_inertial_degree, &previous_error_turn, &integral_turn, 2.0, 0.1, 0.05);
        // left_motors.move_velocity(-output);
        // right_motors.move_velocity(output);

        left_motors.move_velocity(-(std::clamp(output, -90.0, 0.0)));
        right_motors.move_velocity(+(std::clamp(output, -90.0, 0.0)));
        pros::delay(20);
    }
    left_motors.move_velocity(0);
	right_motors.move_velocity(0);
    pros::lcd::set_text(1, "Completed Turn");
    master.set_text(0,0, ("Final deg:" + std::to_string((IMU[0].get_yaw() + IMU[1].get_yaw()) / 2)));
}

void move_backward(double cm)
{
    double distance = cm*23.7;

    double start_rotation_left = 0;
    double start_rotation_right = 0;

    for(int i = 0; i < 3; i++) {
        start_rotation_left += left_motors.get_positions()[i];
        start_rotation_right += right_motors.get_positions()[i];
    }

    start_rotation_left /= 3;
    start_rotation_right /= 3;

    double avg_left_motors = start_rotation_left;
    double avg_right_motors = start_rotation_right;
    while(start_rotation_left > fabs(avg_left_motors - distance)) {
        master.set_text(0, 0, std::to_string(avg_left_motors));
        avg_left_motors = 0;
        avg_right_motors = 0;
        for(int i = 0; i < 3; i++) {
            avg_left_motors += left_motors.get_positions()[i];
            avg_right_motors += right_motors.get_positions()[i];
        }
        avg_left_motors /= 3;
        avg_right_motors /= 3;
        double output = pid((start_rotation_left+distance)-avg_left_motors, &previous_error_turn, &integral_turn, 2.0, 0.1, 0.05);
        left_motors.move(-(std::clamp(output, -60.0, 60.0)));
        right_motors.move(-(std::clamp(output, -60.0, 60.0)));
    }
}
