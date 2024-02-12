#include "main.h"

#include <string>
#include <cmath>

#include "header/kicker.hh"
#include "header/controls.h"
#include "header/devices.hh"
#include "header/drive.hh"
#include "header/functions.hh"
#include "header/ports.h"
#include "header/wings.hh"
#include "header/auto.hh"
#include "header/intake.hh"
#include "header/hang.hh"
#include "header/accel.hh"
double velox = 0;
double veloy = 0;
double veloz = 0;
double dispx = 0;
double dispy = 0;
double dispz = 0;
double pt = pros::millis();

void initialize()
{
	// IMU.reset(true);
	// pros::lcd::initialize();
}

void hold(double holdValue, double holdTime, int delay){
	double startTime = pros::millis();
	double upperLimit = startTime+(holdTime*1000);
	while (pros::millis() < upperLimit){
		pros::lcd::set_text(0, std::to_string(std::clamp((upperLimit-(pros::millis()+((holdTime*1000)-delay)))/1000, 1.0, 127.0)*(holdValue)));
		master.set_text(0 , 0, std::to_string(std::clamp((upperLimit-(pros::millis()+((holdTime*1000)-delay)))/1000, 1.0, 127.0)*(holdValue)));
		kicker_motors.move(std::clamp((upperLimit-(pros::millis()+((holdTime*1000)-delay)))/1000, 1.0, 127.0)*(holdValue));
	}
}

void disabled() 
{
	pros::lcd::clear();
	while (1){
		pros::lcd::set_text(0, "Disabled");
		pros::delay(10);
	}
}

void competition_initialize() {
	//reset_inertial();
}

void autonomous() {

	// Far Side Auton 
	// intake_motors.move(-100);
	// left_motors.move(-110);
	// right_motors.move(-75);
	// pros::delay(1500);
	// intake_motors.move(0);
	// left_motors.move(0);
	// right_motors.move(0);
	// IMU.set_yaw(0);
	// pros::delay(500);
	// right_motors.move(80);
	// left_motors.move(127);
	// pros::delay(400);
	// right_motors.move(0);
	// left_motors.move(0);
	// turn_right_to_look_at(120);
	// intake_motors.move(127);
	// move_forward_inertial(80.0, 70.0, 100.0, 90.0);
	// turn_right_to_look_at(179);
	// right_motors.move(100);
	// left_motors.move(100);
	// pros::delay(500);
	// right_motors.move(0);
	// left_motors.move(0);
	

	// // Far Side Auton 
	// intake_motors.move(-100);
	// left_motors.move(-110);
	// right_motors.move(-75);
	// pros::delay(1500);
	// intake_motors.move(0);
	// left_motors.move(0);
	// right_motors.move(0);
	// IMU.set_yaw(0);
	// pros::delay(500);
	// right_motors.move(80);
	// left_motors.move(127);
	// pros::delay(800);
	// right_motors.move(0);
	// left_motors.move(0);
	// turn_right_to_look_at(79);
	// left_motors.set_zero_position(0);
	// right_motors.set_zero_position(0);
	// move_forward_inertial(80.0, 70.0, 100.0, 90.0);

	// Auton Skills
	intake_motors.move(50);
	kicker_motors.move(-87);
	pros::delay(25000);
	kicker_motors.move(0);
	intake_motors.move(0);
	move_forward_inertial(50.0, 20.0, 50.0, 40.0);
	turn_left_to_look_at(-20);
	left_motors.move(-110);
	right_motors.move(-90);
	pros::delay(2000);
	left_motors.move(0);
	right_motors.move(0);
	master.set_text(0,0, "should reset inertial");
	IMU.set_yaw(0);
	master.set_text(0,0, "reseted inertial");
	move_forward_inertial(10.0, 10.0, 30.0, 20.0);
	turn_right_to_look_at(90);
	move_forward_inertial(250.0, 50.0, 110.0, 90.0);
	turn_left_to_look_at(0);
	move_forward_inertial(80.0, 50.0, 110.0, 90.0);
	wings.set_value(1);
	
	// Auton near (right) side
	// intake_motors.move(-100);
	// left_motors.move(-78);
	// right_motors.move(-120);
	// pros::delay(766);
	// intake_motors.move(0);
	// left_motors.move(0);
	// right_motors.move(0);
	// IMU.set_yaw(0);
	// pros::delay(100);
	// left_motors.move(60);
	// right_motors.move(120);
	// pros::delay(800);
	// left_motors.move(0);
	// right_motors.move(0);
	// turn_left_to_look_at(-90);

	// wings.set_value(1);
	// pros::delay(500);
	// wings.set_value(0);
	// left_motors.move(127);
	// right_motors.move(50);
	// pros::delay(750);
	// left_motors.move(0);
	// right_motors.move(0);
	// pros::delay(100);
	// left_motors.move(-33);
	// right_motors.move(-33);
	// pros::delay(300);
	// left_motors.move(0);
	// right_motors.move(0);
	// reset_inertial();
	// turn_left_to_look_at(-85);
	// double average_inertial_start_angle = IMU.get_yaw();

	// while(ultrasonic.get_value() < 40 || ultrasonic.get_value() > 70) {
    //     double current_inertial_angle = IMU.get_yaw(); 
    //     double error = average_inertial_start_angle - current_inertial_angle;
    //     double output = pid(error, &previous_error_turn, &integral_turn, 7, 0, 0.05);

    //     left_motors.move(std::clamp(20 + output, -20.0, 25.0));
    //     right_motors.move(std::clamp(20 - output, -20.0, 25.0));
    // }
	// left_motors.move(0);
	// right_motors.move(0);
}

void opcontrol() {
	hang.set_value(1);
	pros::Task drive_task(drive_robot);
	pros::Task kicker_task(kicker_trigger);
	pros::Task hanger_task(hang_trigger);
	while (true) {
		handle_intake();
		handle_kicker();
		handle_wings();
		handle_hang();
		pros::delay(10);
	}        
}
