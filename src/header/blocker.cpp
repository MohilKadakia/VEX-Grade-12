#include "main.h"
#include "devices.hh"
#include "controls.h"

bool blocker_active = false;

void blocker_trigger() {
    while(true) {
        if (master.get_digital(master_DOWN)) {
            blocker_active = !blocker_active;
            while (master.get_digital(master_DOWN)) {
                pros::delay(10);
            }
        }
    }
    pros::delay(10);
}

void handle_blocker() {
    if (blocker_active) {
        blocker.set_value(1);
        pros::lcd::set_text(0, "working");
    }
    else {
        blocker.set_value(0);
        pros::lcd::set_text(0, "Not Working");
    }
}