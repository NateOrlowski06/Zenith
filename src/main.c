#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/i2c.h"

#include "header/altimeter.h"
#include "header/state_handler.h"

#include "header/data_logger.h"
#define SERIAL

/*
TODO for tomorrow, experiment with fifo push and pop
Create function for logging data packet into csv format
Make sure to log timestamp of the data packets origin, not when it was put into the file. 
After getting data logging working, clean up main.c


Ideas: Create my own buffer separate from fifo buffer to keep data packets
The data passed to core1 can just be a pointer to the datapacket in the buffer.
*/


// 'main' function for core 1
void core1_entry(){
    struct data_packet* incoming_packet;
    while(1){
        multicore_fifo_pop_blocking();
        log_data(incoming_packet);
    }
}


// core 0
int main()
{
    //usb configuration for serial data
    #ifdef SERIAL
        stdio_init_all();
    #endif

    // starts core1 main loop    
    //multicore_launch_core1(core1_entry);

    struct Altimeter altimeter;
    initialize_altimeter(&altimeter);

    //Used for timing runtime
    absolute_time_t start;
    absolute_time_t end;

    uint8_t state = 1;
    //constant pointer to mutable integer
    uint8_t * const state_pointer = &state;

    initialize_data_logger("data.csv");

    
    

    while(1){
        #ifdef SERIAL
            start = get_absolute_time();
        #endif


        update_altimeter(&altimeter);
        handle_state(state_pointer,&altimeter);
        
        #ifdef SERIAL
            end = get_absolute_time();
        #endif
        
        #ifdef SERIAL
        printf("Altitude: %f    Height: %f   Velocity: %f     State: %d    Execution time: %lld   %f %f\n",altimeter.smooth_altitude,
                                                                                                     altimeter.height,
                                                                                                     altimeter.smooth_velocity,
                                                                                                     state,
                                                                                                     absolute_time_diff_us(start,end),
                                                                                                     altimeter.max_height,
                                                                                                     altimeter.max_velocity);
        #endif

        struct data_packet data_packet = {
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
        //multicore_fifo_push_blocking(1);
        log_data(&data_packet);
    }



    return 0;
}
