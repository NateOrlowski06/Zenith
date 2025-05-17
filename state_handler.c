#include "state_handler.h"

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
    //log data, perhaps altitude predictor algorithm, pretend to deploy ejection charges upon next state
    *state = *state << ((altimeter -> max_height > altimeter -> height) && (altimeter -> smooth_velocity < FREEFALL_VELOCITY_THRESHOLD));
}

void handle_freefall(uint8_t *state, struct Altimeter* altimeter){
    //Log data

    *state = *state << ((altimeter -> height < LANDED_HEIGHT_THRESHOLD) || ((altimeter -> smooth_velocity) < LANDED_VELOCITY_THRESHOLD));
}

void handle_landed(uint8_t *state, struct Altimeter* altimeter){
    //Close data file
}

