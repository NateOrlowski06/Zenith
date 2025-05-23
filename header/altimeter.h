#ifndef ALTIMETER_H
#define ALTIMETER_H

#include "header/bmp180.h"
#include "constants.h"
/*
Root mean square (RMS) variance is a funciton of the Over Sampling Setting (OSS) on the BMP180.
With OSS zero, RMS is 0.5 m
Therefore, the moving average variance is plus or minus RMS/list_size
Variance = RMS/list_size = 0.1 m

dH is the height change during time interval for a full refresh of values
At OSS 0, sensor takes 4.5 ms per pressure and 4.5ms per temp, so 9 total
dH = OSS_Delay * speed(m/s) * list_size

small dH correlates to larger variance, vice versa
dH can be used to ensure the altimeter is responsive enough for the given task
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
    absolute_time_t start_of_flight;
};



void update_smooth_altitude(struct Altimeter * altimeter);
void update_smooth_velocity(struct Altimeter * altimeter);


void initialize_altimeter(struct Altimeter * altimeter);

/*

    This function is called by main.c ever loop iteration and passed a pointer to altimeter struct
    It first gets most recent reading from BMP180, takes 9 ms
    Then it adds this value to the list in update_smooth_altitude
    Then adds new velocity value to the list

*/
inline void update_altimeter(struct Altimeter * altimeter){
    bmp_get_pressure_temperature(&(altimeter -> bmp180));
    // Order matters, altitude must be calculated before velocity for lagging pointer logic
    update_smooth_altitude(altimeter);
    update_smooth_velocity(altimeter);
}
#endif