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

inline void next_state(uint8_t *state){ *state << 1;}

void handle_standby(const uint8_t *state, struct Altimeter* altimeter);
void handle_boost(const uint8_t *state, struct Altimeter* altimeter);
void handle_coast(const uint8_t *state, struct Altimeter* altimeter);
void handle_freefall(const uint8_t *state, struct Altimeter* altimeter);
void handle_landed(const uint8_t *state, struct Altimeter* altimeter);

void (*state_handlers[5])(const uint8_t *state, struct Altimeter* altimeter) = {handle_standby,
                                                                       handle_boost,
                                                                       handle_coast,
                                                                       handle_freefall,
                                                                       handle_landed};






