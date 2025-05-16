#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "altimeter.h"



int main()
{
    //usb configuration for serial data
    stdio_init_all();


    //Waits for user input to start program, for development and testing only
    while(getchar() != 'X'){
        sleep_ms(10);
    }

    struct Altimeter altimeter;
    initialize_altimeter(&altimeter);

    absolute_time_t start;
    absolute_time_t end;

    int trigger = 0;
    while(1){
        start = get_absolute_time();
        update_altimeter(&altimeter);
        end = get_absolute_time();
        printf("Altitude: %f    Velocity: %f     Trigger: %d    Execution time: %lld\n",altimeter.smooth_altitude,altimeter.smooth_velocity,trigger,absolute_time_diff_us(start,end));
        
        if(altimeter.smooth_velocity > 3){
            trigger = 1;
        }
    }



    return 0;
}
