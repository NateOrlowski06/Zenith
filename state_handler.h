#ifndef STATE_HANDLER_H
#define STATE_HANDLER_H

#include <stdint.h>
#include "altimeter.h"
#define STANDBY 1
#define BOOST 2
#define COAST 4
#define FREEFALL 8
#define LANDED 16

#define TAKEOFF_HEIGHT_THRESHOLD 10
#define TAKEOFF_VELO_THRESHOLD 5
#define MOTOR_BURNOUT_PERCENT 0.95
#define FREEFALL_VELOCITY_THRESHOLD -5
#define LANDED_VELOCITY_THRESHOLD 5
#define LANDED_HEIGHT_THRESHOLD 5


void handle_state(uint8_t *state, struct Altimeter* altimeter);

void handle_standby(uint8_t *state, struct Altimeter* altimeter);
void handle_boost(uint8_t *state, struct Altimeter* altimeter);
void handle_coast(uint8_t *state, struct Altimeter* altimeter);
void handle_freefall(uint8_t *state, struct Altimeter* altimeter);
void handle_landed(uint8_t *state, struct Altimeter* altimeter);

#endif




