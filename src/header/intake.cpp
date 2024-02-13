#include "main.h"
#include "devices.hh"
#include "controls.h"


void intake() {
    while(true) {
		bool intake_forward = master.get_digital(master_R2);
		bool intake_backward = master.get_digital(master_L2);

        if (intake_forward && intake_backward) {
            intake_motor.brake();
        }
        else if (intake_forward) {
            intake_motor.move(127);
        }
        else if (intake_backward) {
            intake_motor.move(-127);
        }
        else {
            intake_motor.brake();
        }
        pros::delay(10);
    }
}
void handle_intake() {
    while (master.get_digital(master_L1)) {
        intake_motors.move(127);
    }

    while (master.get_digital(master_R1)) {
        intake_motors.move(-127);
    }

    if (!(master.get_digital(master_R1) && master.get_digital(master_L1))) {
        intake_motors.move(0);
        pros::lcd::set_text(0, "Not in action");
    }
}
