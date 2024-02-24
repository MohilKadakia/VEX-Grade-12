#include "main.h"
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

void initialize()
{
	// pros::lcd::initialize();
	IMU.reset(true);
	left_motors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
	right_motors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
}

void disabled() 
{
	pros::lcd::clear();
}

void competition_initialize() {
}

void autonomous() {
	// move_forward_inertial_pid(15, -127, 100);
	pros::delay(50);
	left_motors.move(92);
	right_motors.move(100);
	pros::delay(100);
	left_motors.move(0);
	right_motors.move(0);
	pros::delay(100);
	turn(-90, 1.2, 0.2, 1.6);
	left_motors.move(88);
	right_motors.move(94);
	intake_motor.move(127);
	pros::delay(760); // At Post
	left_motors.move(0);
	right_motors.move(0);
	pros::delay(1100);
	intake_motor.move(10);
	left_motors.move(-92);
	right_motors.move(-100);
	pros::delay(750); //Drive Back
	left_motors.move(0);
	right_motors.move(0);
	turn(135, 1.2, 0.2, 1.6); //Turn to net
	intake_motor.move(-127);
	left_motors.move(88);
	right_motors.move(94);
	pros::delay(400); // Push towards net
	left_motors.move(0);
	right_motors.move(0);
	// left_motors.move(92);
	// right_motors.move(127);
	// pros::delay(750);
	// left_motors.move(-20);
	// right_motors.move(-25);
	pros::delay(200);
	turn(-110, 1.2, 0.2, 1.6); // Turn to mid
	left_motors.move(88);
	right_motors.move(94);
	pros::delay(500);
	left_motors.move(0);
	right_motors.move(0);
	turn(25, 1.2, 0.2, 1.6); // Thread the needle
	left_motors.move(70);
	right_motors.move(78);
	intake_motor.move(127);
	pros::delay(1300); //Pick up triball
	left_motors.move(0);
	right_motors.move(0);
	pros::delay(100);
	turn(120, 1.2, 0.2, 1.6); // Turn to net
	pros::delay(200);
	intake_motor.move(-127);
	left_motors.move(100);
	right_motors.move(110);
	pros::delay(900); // Push into net
	left_motors.move(-88);
	right_motors.move(-98);
	pros::delay(300); // Drive away
	left_motors.move(0);
	right_motors.move(0);
	turn(150, 1.2, 0.2, 1.6); // turn to corner ball
	intake_motor.move(127);
	left_motors.move(80);
	right_motors.move(90);
	pros::delay(850);// Drive to cornet ball
	left_motors.move(0);
	right_motors.move(0);
	pros::delay(100);
	turn(-110, 1.2, 0.2, 1.6); // turn to og corner
	left_motors.move(80);
	right_motors.move(90);
	pros::delay(1400); // Drive there
	left_motors.move(0);
	right_motors.move(0);
	turn(-150, 1.2, 0.2, 1.6); // turn to og corner
	intake_motor.move(-127);
	left_motors.move(80);
	right_motors.move(90);
	pros::delay(1000); // Dr
	left_motors.move(-50);
	right_motors.move(-50);
	pros::delay(200);
	left_motors.move(110);
	right_motors.move(127);
	pros::delay(700);
	intake_motor.move(0);

	



	// move_forward_inertial_pid(100, -1, 100);

}

void opcontrol() {
	hang.set_value(1);
	left_motors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
	right_motors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
	pros::Task kicker_task(kicker_trigger);
	pros::Task hanger_task(hang_trigger); 
	pros::Task wings_task(wings_trigger);
	pros::Task driver_options_task(driver_options);
	while (true) {
		drive_robot();
		handle_intake();
		handle_kicker();
		handle_wings();
		handle_hang();
		// hold_kicker(0,3);
		pros::lcd::set_text(0, std::to_string(kicker_motor.get_efficiency()));
		master.set_text(0, 0,  std::to_string(kicker_motor.get_efficiency()));

		pros::delay(16);
	}        
}
