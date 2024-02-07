#include "main.h"
#include "devices.hh"
#include "controls.h"

bool wings_active = false;

void wings_trigger() {
    while(true) {
        if (master.get_digital(master_L1) || master.get_digital(master_R1)) {
            wings_active = !wings_active;
            while (master.get_digital(master_L1) || master.get_digital(master_R1)) {
                pros::delay(10);
            }
        }
    }
    pros::delay(10);
}

void handle_wings() {
    while ((master.get_digital(master_L2)) || (master.get_digital(master_R2))) {
        wings.set_value(1);
        pros::delay(100);
    }


    if (!(master.get_digital(master_R2) && master.get_digital(master_L2))) {
        wings.set_value(0);
        pros::delay(100);
    }
}