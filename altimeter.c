#include "altimeter.h"


int initialize_altimeter(struct Altimeter * altimeter){
    //oss [0-3], 0 is fastest but least accurate, 3 is opposite
    altimeter -> bmp180.oss = 0;
    altimeter -> bmp180.i2c.addr = 0x77;
    altimeter -> bmp180.i2c.inst = I2C_PORT;
    altimeter -> bmp180.i2c.rate = 400000;
    altimeter -> bmp180.i2c.scl = I2C_SCL;
    altimeter -> bmp180.i2c.sda = I2C_SDA;

    //Initializes I2C connection
    return bmp_init(&(altimeter -> bmp180));
}


void update_altimeter(struct Altimeter * altimeter){
    bmp_get_pressure_temperature(&(altimeter -> bmp180));
    update_smooth_altitude(altimeter);
    update_smooth_velocity(altimeter);
}

void update_smooth_altitude(struct Altimeter * altimeter){
    altimeter -> smooth_altitude = 0.0;
}

void update_smooth_velocity(struct Altimeter * altimeter){
    altimeter -> smooth_velocity = 0.0;
}
