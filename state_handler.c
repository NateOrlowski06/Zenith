#include "state_handler.h"
#include <time.h>
void (*state_handlers[5])(uint8_t *state, struct Altimeter* altimeter) = {handle_standby,
                                                                       handle_boost,
                                                                       handle_coast,
                                                                       handle_freefall,
                                                                       handle_landed};


inline void handle_state(uint8_t *state, struct Altimeter* altimeter){

    state_handlers[__builtin_ctz(*state)](state, altimeter);
}

void handle_standby(uint8_t *state, struct Altimeter* altimeter){
    //Create data buffer so we can log 5 seconds before flight starts

    //Shifts the bit to the left by either 1 or zero each time
    *state = *state << (altimeter -> height > TAKEOFF_HEIGHT_THRESHOLD && 
               altimeter -> smooth_velocity > TAKEOFF_VELO_THRESHOLD);
}

void handle_boost(uint8_t *state, struct Altimeter* altimeter){
    //Log data

    *state = *state << ((altimeter -> max_velocity * MOTOR_BURNOUT_PERCENT) > (altimeter -> smooth_velocity));
}

void handle_coast(uint8_t *state, struct Altimeter* altimeter){
    //Moves the state back to standby if the altitude is less than expected by coasting state
    *state = *state >> 2*((altimeter -> smooth_altitude) < SAFE_ARMING_HEIGHT);
    altimeter -> is_armed = ARM;

    *state = *state << ((altimeter -> max_height > altimeter -> height) && (altimeter -> smooth_velocity < FREEFALL_VELOCITY_THRESHOLD));
}

void handle_freefall(uint8_t *state, struct Altimeter* altimeter){
    //Freefall_start initializied to zero statically, updated on first funciton call
    static absolute_time_t freefall_start_time = 0;
    static absolute_time_t main_deployment_time = 0;
    //only ever evaluated to get_absolute_time when freefall_start is zero, otherwise, it is never changed again
    freefall_start_time = (freefall_start_time == 0) * get_absolute_time();

    //GPIO is set to on if it has been less than 2 seconds from freefall detection and altimeter is armed
    gpio_put(DROGUE_CHARGE_PIN, (absolute_time_diff_us(freefall_start_time, get_absolute_time()) < PULSE_DURATION * US_TO_SEC) && 
                                 altimeter -> is_armed);


   // int main_deployment_condition = (absolute_time_diff_us(freefall_start_time, get_absolute_time()) > (PULSE_DURATION + 1) * US_TO_SEC) && 
     //                         (altimeter -> smooth_altitude < MAIN_DEPLOYMENT_HEIGHT) && 
       //                       (altimeter -> is_armed);
    int main_deployment_condition =  
                              (altimeter -> height < MAIN_DEPLOYMENT_HEIGHT) && 
                              (altimeter -> is_armed);
 
    gpio_put(MAIN_CHARGE_PIN, main_deployment_condition);

    *state = *state << ((altimeter -> height < LANDED_HEIGHT_THRESHOLD) && ((altimeter -> smooth_velocity) > LANDED_VELOCITY_THRESHOLD));
}

void handle_landed(uint8_t *state, struct Altimeter* altimeter){
    gpio_put(DROGUE_CHARGE_PIN,0);
    gpio_put(MAIN_CHARGE_PIN,0);
    altimeter -> is_armed = DISARM;
    //Close data file
}

