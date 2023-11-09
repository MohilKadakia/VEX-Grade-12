#include "main.h"
#include "devices.hh"
#include "controls.h"

bool wings_active = false;

void wings_trigger() {
    while(true) {
        if (master.get_digital(master_L2)) {
            wings_active = !wings_active;
            while (master.get_digital(master_L2)) {
                pros::delay(10);
            }
        }
    }
    pros::delay(10);
}
void handle_wings()
{
    if (wings_active)
    {   
        wings.set_value(true);
    }
    else
    {
        wings.set_value(false);
    }
}