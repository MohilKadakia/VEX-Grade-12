#include "main.h"
#include "controls.h"
#include "devices.hh"

bool catapult_active = false;

void debug_values()
{
	pros::lcd::set_text(0, "Displacement: LMtop:" + std::to_string(left_motor_1.get_encoder_units()) + " LMfro" + std::to_string(left_motor_2.get_encoder_units()) + " LMbac" + std::to_string(left_motor_3.get_encoder_units()));
	pros::lcd::set_text(1, "Displacement: RMtop:" + std::to_string(right_motor_1.get_encoder_units()) + " RMfro" + std::to_string(right_motor_2.get_encoder_units()) + " RMbac" + std::to_string(right_motor_3.get_encoder_units()));
	pros::lcd::set_text(2, "RPM: LMtop:" + std::to_string(left_motor_1.get_actual_velocity()) + " LMfro" + std::to_string(left_motor_2.get_actual_velocity()) + " LMbac" + std::to_string(left_motor_3.get_actual_velocity()));
	pros::lcd::set_text(3, "RPM: RMtop:" + std::to_string(right_motor_1.get_actual_velocity()) + " RMfro" + std::to_string(right_motor_2.get_actual_velocity()) + " RMbac" + std::to_string(right_motor_3.get_actual_velocity()));
	pros::lcd::set_text(4, "Yaw: ISfro" + std::to_string(IMU[0].get_yaw()) + "ISbac" + std::to_string(IMU[1].get_yaw()));
	pros::lcd::set_text(5, "Catapult: " + std::to_string(catapult_active));
}

void catapult_trigger()
{	
	while(true) {
		if (master.get_digital(master_R2))
		{
			catapult_active = !catapult_active;
			while(master.get_digital(master_R2)) {
				pros::delay(10);
			}
		}
		pros::delay(10);
	}
}
void handle_catapult()
{
	if (catapult_active)
	{
		catapult_motors.move(75);
	}
	else
	{
		catapult_motors.move(0);
	}
}