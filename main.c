#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "altimeter.h"


#define LED_PIN 16

int main()
{
    //usb configuration for serial data
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN,1);

    //Waits for user input to start program, for development and testing only
    while(getchar() != 'X'){
        sleep_ms(10);
    }

    struct Altimeter altimeter;
    initialize_altimeter(&altimeter);
  
    gpio_put(LED_PIN,1);
   
    while(1){
        update_altimeter(&altimeter);
        printf("%f\n",altimeter.smooth_altitude);
    }

    return 0;
}
