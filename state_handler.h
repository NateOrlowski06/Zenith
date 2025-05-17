#include <stdint.h>
#define STANDBY 1
#define BOOST 2
#define COAST 4
#define FREEFALL 8
#define LANDED 16


inline void next_state(uint8_t *state){ *state << 1;}

void handle_standby(const uint8_t *state);
void handle_boost(const uint8_t *state);
void handle_coast(const uint8_t *state);
void handle_freefall(const uint8_t *state);
void handle_landed(const uint8_t *state);

void (*state_handlers[5])(uint8_t state) = {handle_standby,
                                            handle_boost,
                                            handle_coast,
                                            handle_freefall,
                                            handle_landed};






