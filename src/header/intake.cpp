#include "main.h"
#include "devices.hh"
#include "controls.h"


void intake() {
    // while(true) {
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
        // pros::delay(10);

    // }
}
