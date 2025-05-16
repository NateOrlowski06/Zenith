#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "altimeter.h"


#define LED_PIN 16
void led_init(void);

int main()
{
    //usb configuration for serial data
    stdio_init_all();
    led_init();

    while(getchar() != 'X'){
        sleep_ms(10);
    }

    struct Altimeter altimeter;
    
    if(!initialize_altimeter(&altimeter)){
        return 1; 
    }
  

    gpio_put(LED_PIN,1);

    while(1){
        
        update_altimeter(&altimeter);
        printf("%f\n",altimeter.bmp180.altitude);
        //update state
        
        
    }

    return 0;
}

void led_init(void){
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN,1);
}

