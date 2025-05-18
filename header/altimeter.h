#ifndef ALTIMETER_H
#define ALTIMETER_H

#include "header/bmp180.h"
#include "constants.h"
/*
RMS variance is a funciton of OSS, with OSS zero, RMS is 0.5 m
Therefore, the moving average variance is plus or minus RMS/list_size
V = RMS/list_size = 0.1 m

dH is the height change during time interval for a full refresh of values
At OSS 0, sensor takes 5 ms per pressure and 5ms per temp, so 10 total
dH = OSS_Delay * speed(m/s) * list_size

small dH correlates to larger variance, vice versa
*/



struct Altitude_Node{
    float value;
    absolute_time_t time; 
    struct Altitude_Node * next_address;
};

struct Velocity_Node{
    float value;
    struct Velocity_Node * next_address;
};

struct Altimeter{
    bmp_t bmp180;
    struct Altitude_Node altitude_readings[LINKED_LIST_SIZE];
    struct Velocity_Node velocity_calculations[LINKED_LIST_SIZE];
    float smooth_altitude;
    float smooth_velocity;
    struct Altitude_Node * altitude_pointer;
    struct Velocity_Node * velocity_pointer;
    struct Altitude_Node * lagging_pointer;
    float max_velocity;
    float max_height;
    float height;
    float initial_altitude;
    uint8_t is_armed;
};


void update_altimeter(struct Altimeter * altimeter);

void update_smooth_altitude(struct Altimeter * altimeter);
void update_smooth_velocity(struct Altimeter * altimeter);


void initialize_altimeter(struct Altimeter * altimeter);

#endif