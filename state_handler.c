#include "state_handler.h"

void handle_standby(const uint8_t *state, struct Altimeter* altimeter){
    //Create data buffer so we can log 5 seconds before flight starts

    //Shifts the bit to the left by either 1 or zero each time
    *state << (altimeter -> smooth_altitude > TAKEOFF_HEIGHT_THRESHOLD && 
               altimeter -> smooth_velocity > TAKEOFF_VELO_THRESHOLD);
}

void handle_boost(const uint8_t *state, struct Altimeter* altimeter){
    //Log data

    *state << (altimeter -> max_velocity > altimeter -> smooth_velocity * MOTOR_BURNOUT_PERCENT);
}

void handle_coast(const uint8_t *state, struct Altimeter* altimeter){
    //log data, perhaps altitude predictor algorithm, pretend to deploy ejection charges upon next state

    *state << (altimeter -> max_height > altimeter -> height) && (altimeter -> smooth_velocity < FREEFALL_VELOCITY_THRESHOLD);
}

void handle_freefall(const uint8_t *state, struct Altimeter* altimeter){
    //Log data

    *state << (altimeter -> height < LANDED_HEIGHT_THRESHOLD) || ((altimeter -> smooth_velocity) < LANDED_VELOCITY_THRESHOLD);
}

void handle_landed(const uint8_t *state, struct Altimeter* altimeter){
    //Close data file
}

