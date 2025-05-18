#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "header/altimeter.h"
#include "header/state_handler.h"
//#define SERIAL

/*

    Kind of a mess right now
    in general, it just repeatedly calls the functions to update the altimeter values
    and the funcions to handle the states

    Everything else is for debugging, timing, and serial output

*/


int main()
{
    //usb configuration for serial data
    #ifdef SERIAL
        stdio_init_all();
    #endif


    struct Altimeter altimeter;
    initialize_altimeter(&altimeter);

    //Used for timing runtime
    absolute_time_t start;
    absolute_time_t end;

    uint8_t state = 1;
    //constant pointer to mutable integer
    uint8_t * const state_pointer = &state;

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
    }



    return 0;
}
