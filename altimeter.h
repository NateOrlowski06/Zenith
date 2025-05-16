
#include "Libs/bmp180.h"

/*
RMS variance is a funciton of OSS, with OSS zero, RMS is 0.5 m
Therefore, the moving average variance is plus or minus RMS/list_size
V = RMS/list_size = 0.1 m

dH is the height change during time interval for a full refresh of values
At OSS 0, sensor takes 5 ms per reading
dH = OSS_Delay * speed(m/s) * list_size

small dH correlates to larger variance, vice versa
*/

#define LINKED_LIST_SIZE 5

#define I2C_PORT i2c0
#define I2C_SDA 0
#define I2C_SCL 1

struct Node{
    float value;
    struct Node * next_address;
};

struct Altimeter{
    bmp_t bmp180;
    struct Node altitude_readings[LINKED_LIST_SIZE];
    struct Node velocity_calculations[LINKED_LIST_SIZE];
    float smooth_altitude;
    float smooth_velocity;
    struct Node * altitude_pointer;
    struct Node * velocity_pointer;
};


void update_altimeter(struct Altimeter * altimeter);

void update_smooth_altitude(struct Altimeter * altimeter);
void update_smooth_velocity(struct Altimeter * altimeter);


void initialize_altimeter(struct Altimeter * altimeter);
