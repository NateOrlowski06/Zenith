#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H




#include "hw_config.h"
#include "f_util.h"
#include "ff.h"

#include "altimeter.h"
#include <time.h>
#include "pico/util/queue.h"
#include "constants.h"
#include "state_handler.h"


struct data_packet{
	double time;
	
	int32_t raw_pressure;
	float temperature;
	
	float raw_altitude;
	float smooth_altitude;
	float height;
	float velocity;
	float smooth_velocity;
	uint8_t is_armed;

	uint8_t state;

};

extern queue_t packet_queue;
extern struct data_packet packet;

void initialize_data_logger();
void log_data(struct data_packet*);
void end_logging();
void make_packet(struct Altimeter*, uint8_t);
#endif

