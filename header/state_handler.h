#ifndef STATE_HANDLER_H
#define STATE_HANDLER_H

#include <stdint.h>
#include "altimeter.h"
#include "constants.h"

#include "header/data_logger.h"
#include "pico/multicore.h"
#include <time.h>



void handle_state(uint8_t *state, struct Altimeter* altimeter);

void handle_standby(uint8_t *state, struct Altimeter* altimeter);
void handle_boost(uint8_t *state, struct Altimeter* altimeter);
void handle_coast(uint8_t *state, struct Altimeter* altimeter);
void handle_freefall(uint8_t *state, struct Altimeter* altimeter);
void handle_landed(uint8_t *state, struct Altimeter* altimeter);

extern absolute_time_t start_of_flight;

#endif




