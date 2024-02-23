#ifndef _kicker_HH_
#define _kicker_HH_
extern bool kicker_active;
void debug_values();
void kicker_trigger();
void handle_kicker();
void hold_kicker(int hold_value, int efficentcy_threshold);
#endif