#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/i2c.h"
#include "pico/util/queue.h"

#include "header/altimeter.h"
#include "header/state_handler.h"

#include "header/data_logger.h"

/*
TODO: Start logging at beginning of flight
make timestamp a measure of time from beginning of flight

*/

// Data logging queue to be used by both cores
queue_t packet_queue;

// 'main' function for core 1
void core1_entry(){
    struct data_packet incoming_packet;
    while(1){
        queue_remove_blocking(&packet_queue, (void*)&incoming_packet);
        log_data(&incoming_packet);
    }
}


// core 0
int main()
{

    struct Altimeter altimeter;
    initialize_altimeter(&altimeter);


    uint8_t state = 1;
    // constant pointer to mutable integer
    uint8_t * const state_pointer = &state;

    initialize_data_logger();

    queue_init_with_spinlock(&packet_queue, sizeof(struct data_packet), DATA_PACKETS_QUEUE_SIZE, 1);
    

    multicore_launch_core1(core1_entry);

    while(1){
        update_altimeter(&altimeter);
        handle_state(state_pointer,&altimeter);

        struct data_packet packet = {
            get_absolute_time(),
            altimeter.bmp180.pressure,
            altimeter.bmp180.temperature,
            altimeter.altitude_pointer -> value,
            altimeter.smooth_altitude,
            altimeter.height,
            altimeter.velocity_pointer -> value,
            altimeter.smooth_velocity,
            altimeter.is_armed,
            state
        };

        queue_try_add(&packet_queue, (void*)&packet);
    }

    return 0;
}
