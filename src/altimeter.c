#include "header/altimeter.h"
#include "header/constants.h"
/*

    This function is called by main.c once and passed a pointer to altimeter struct

    Initializes basically everything; I2C, BMP180, lists, pointers, variables, and GPIO
*/
void initialize_altimeter(struct Altimeter * altimeter){
    // oss [0-3], 0 is fastest but least accurate, 3 is opposite
    altimeter -> bmp180.oss = 0;
    altimeter -> bmp180.i2c.addr = 0x77;
    altimeter -> bmp180.i2c.inst = I2C_PORT;
    altimeter -> bmp180.i2c.rate = 400000;
    altimeter -> bmp180.i2c.scl = I2C_SCL;
    altimeter -> bmp180.i2c.sda = I2C_SDA;

    // initializes I2C connection
    bmp_init(&(altimeter -> bmp180));
    bmp_get_pressure_temperature(&(altimeter -> bmp180));

    // Creates circular linked list for easy traversal during data aquisition and averaging
    // Initializes all altitude with current altitude so zeroes do not affect velocity
    for(int i = 0; i<LINKED_LIST_SIZE-1; i++){
        altimeter -> altitude_readings[i].value = altimeter -> bmp180.altitude;
        altimeter -> altitude_readings[i].next_address = &(altimeter -> altitude_readings[i+1]);

        // Velocity assumed to be zero upon power on, will get updated to small number near zero
        // during the first iteration of calculations.
        altimeter -> velocity_calculations[i].value = 0;
        altimeter -> velocity_calculations[i].next_address = &(altimeter -> velocity_calculations[i+1]);

        altimeter -> altitude_readings[i].time = get_absolute_time();
    }    


    // Points tail of list to head, finalizing the circular list
    altimeter -> altitude_readings[LINKED_LIST_SIZE-1].value = altimeter -> bmp180.altitude; //replace with: altitude_readings
    altimeter -> altitude_readings[LINKED_LIST_SIZE-1].next_address = &(altimeter -> altitude_readings[0]);
    
    altimeter -> velocity_calculations[LINKED_LIST_SIZE-1].value = 0;
    altimeter -> velocity_calculations[LINKED_LIST_SIZE-1].next_address = &(altimeter -> velocity_calculations[0]);

    altimeter -> altitude_pointer = &(altimeter-> altitude_readings[0]);
    altimeter -> velocity_pointer = &(altimeter-> velocity_calculations[0]);
    altimeter -> lagging_pointer  = &(altimeter-> altitude_readings[0]);

    altimeter -> initial_altitude = altimeter -> bmp180.altitude;
    altimeter -> height = 0;
    altimeter -> max_height = 0;
    altimeter -> max_velocity = 0;

    gpio_init(DROGUE_CHARGE_PIN);
    gpio_init(MAIN_CHARGE_PIN);
    gpio_set_dir(MAIN_CHARGE_PIN, 1);
    gpio_set_dir(DROGUE_CHARGE_PIN, 1);

    altimeter -> is_armed = 0;
}



/*

    Called by update_altimeter and passed a pointer to altimeter struct
    The altitude readings are noisy so a moving average is employed here
    The circular linked list makes the logic easy for traversing the array and overriting old values.

*/
void update_smooth_altitude(struct Altimeter * altimeter){

    // "Value in the current node to equal the value the sensor just recorded"
    altimeter -> altitude_pointer -> value = altimeter -> bmp180.altitude;
    altimeter -> altitude_pointer -> time = get_absolute_time();

    // Initializes sum variable and temporary pointer for traversing the list
    float sum = 0;
    struct Altitude_Node * loop_pointer = altimeter -> altitude_pointer;

    // 1: Add current node to sum
    // 2: Increment to next node
    // 3: Stops after it reaches current altitude pointer
    do {
        sum += loop_pointer -> value;
        loop_pointer = loop_pointer -> next_address;
    } while(loop_pointer != altimeter-> altitude_pointer);
    
    // "The address of the new node is now equal to the address being pointed to"
    altimeter -> altitude_pointer = altimeter -> altitude_pointer -> next_address;

    altimeter -> smooth_altitude = sum/LINKED_LIST_SIZE;
    altimeter -> height = (altimeter -> smooth_altitude) - (altimeter -> initial_altitude);

    if(altimeter -> height > altimeter -> max_height){
        altimeter -> max_height = altimeter -> height;
    }
}

/*
    
    Called by update_altimeter and passed a pointer to altimeter struct

    General overview: The average velocity is taken between the oldest and newest members of altitude_readings
    that value is then put into its own circular linked list so that we can have a velocity moving average

    The lagging pointer is a pointer to an altitude node that 'stays behind' the current altitude node pointer
    In the runtime sequence, the oldest altitude is replaced with the newest, the altitude pointer is then incremented
    to the oldest altitude so that it can be updated upon the next iteration. Therefore, by keeping a lagging pointer
    one step behind the altitude pointer, we have access to the newest and oldest altitude values without using a doubly linked list

*/
void update_smooth_velocity(struct Altimeter * altimeter){

    double dt = absolute_time_diff_us(altimeter -> lagging_pointer -> time, altimeter -> altitude_pointer -> time) / US_TO_SEC;
    float height_difference =  (altimeter -> altitude_pointer -> value) - (altimeter -> lagging_pointer -> value);
    
    // Lagging pointer catches back up to altitude pointer, but will lag behind in update_smooth_altitude
    altimeter -> lagging_pointer = altimeter -> altitude_pointer;
    altimeter -> velocity_pointer -> value = height_difference / (LINKED_LIST_SIZE * dt);

    float sum = 0;
    // Temporary variable for traversing array
    struct Velocity_Node * loop_pointer = altimeter -> velocity_pointer;
    
    do{
        sum += loop_pointer -> value;
        loop_pointer = loop_pointer -> next_address;
    } while(loop_pointer != altimeter -> velocity_pointer);

    altimeter -> velocity_pointer = altimeter -> velocity_pointer -> next_address;
    altimeter -> smooth_velocity = sum/LINKED_LIST_SIZE;
    
    if(altimeter -> max_velocity > altimeter -> smooth_velocity){
        altimeter -> max_velocity = altimeter -> smooth_velocity;
    }

}


