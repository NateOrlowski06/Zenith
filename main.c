#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "bmp180.h"



// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 0
#define I2C_SCL 1





int main()
{
    stdio_init_all();

    while(getchar() != 'X'){
        sleep_ms(10);
    }


    bmp_t sensor;
    sensor.oss = 0;
    sensor.i2c.addr = 0x77;
    sensor.i2c.inst = I2C_PORT;
    sensor.i2c.rate = 400000;
    sensor.i2c.scl = I2C_SCL;
    sensor.i2c.sda = I2C_SDA;

    if (!bmp_init(&sensor)){
        return 1;
    }



    

    while(1){
        

        bmp_get_pressure_temperature(&sensor);


        float altitude = sensor.altitude;
        //int32_t pressure = sensor.pressure;
        //float temperature = sensor.temperature;

        printf("Altitude: %f\n",altitude);
        //printf("Pressure: %d\n",pressure);
        //printf("Temp: %f\n\n",temperature);
        
        
        
    }

    return 0;
}
