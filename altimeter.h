
#include "Libs/bmp180.h"

#define RING_BUFFER_SIZE 10

#define I2C_PORT i2c0
#define I2C_SDA 0
#define I2C_SCL 1


struct Altimeter{
    bmp_t bmp180;
    float altitude_readings[RING_BUFFER_SIZE];
    float velocity_calculations[RING_BUFFER_SIZE];
    float smooth_altitude;
    float smooth_velocity;
};


void update_altimeter(struct Altimeter * altimeter);

void update_smooth_altitude(struct Altimeter * altimeter);
void update_smooth_velocity(struct Altimeter * altimeter);


int initialize_altimeter(struct Altimeter * altimeter);
