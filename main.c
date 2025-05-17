#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "altimeter.h"
#include "state_handler.h"
//#define SERIAL

int main()
{
    //usb configuration for serial data
    #ifdef SERIAL
        stdio_init_all();
    #endif


    struct Altimeter altimeter;
    initialize_altimeter(&altimeter);

    absolute_time_t start;
    absolute_time_t end;

    uint8_t state = 1;
    //constant pointer to mutable integer
    uint8_t * const state_pointer = &state;

    while(1){
        start = get_absolute_time();
        update_altimeter(&altimeter);

        handle_state(state_pointer,&altimeter);        
        end = get_absolute_time();
        
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
