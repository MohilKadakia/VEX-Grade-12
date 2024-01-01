#include "main.h"
#include "devices.hh"
#include "controls.h"

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