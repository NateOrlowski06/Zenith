#include "altimeter.h"


void initialize_altimeter(struct Altimeter * altimeter){
    //oss [0-3], 0 is fastest but least accurate, 3 is opposite
    altimeter -> bmp180.oss = 0;
    altimeter -> bmp180.i2c.addr = 0x77;
    altimeter -> bmp180.i2c.inst = I2C_PORT;
    altimeter -> bmp180.i2c.rate = 400000;
    altimeter -> bmp180.i2c.scl = I2C_SCL;
    altimeter -> bmp180.i2c.sda = I2C_SDA;

    //initializes I2C connection
    bmp_init(&(altimeter -> bmp180));

    //Creates circular linked list for easy traversal during data aquisition and averaging
    //Initializes all altitude with current altitude so zeroes do not affect velocity
    for(int i = 0; i<LINKED_LIST_SIZE-1; i++){
        altimeter -> altitude_readings[i].value = altimeter -> bmp180.altitude;
        altimeter -> altitude_readings[i].next_address = &(altimeter -> altitude_readings[i+1]);

        //Velocity assumed to be zero upon power on, will get updated to small number near zero
        //during the first iteration of calculations.
        altimeter -> velocity_calculations[i].value = 0;
        altimeter -> velocity_calculations[i].next_address = &(altimeter -> velocity_calculations[i+1]);
    }

    
    //Points tail of list to head, finalizing the circular list
    altimeter -> altitude_readings[LINKED_LIST_SIZE-1].value = altimeter -> bmp180.altitude; //replace with: altitude_readings
    altimeter -> altitude_readings[LINKED_LIST_SIZE-1].next_address = &(altimeter -> altitude_readings[0]);
    
    altimeter -> velocity_calculations[LINKED_LIST_SIZE-1].value = 0;
    altimeter -> velocity_calculations[LINKED_LIST_SIZE-1].next_address = &(altimeter -> velocity_calculations[0]);

    altimeter -> altitude_pointer = &(altimeter-> altitude_readings[0]);
    altimeter -> velocity_pointer = &(altimeter-> velocity_calculations[0]);

}


void update_altimeter(struct Altimeter * altimeter){
    bmp_get_pressure_temperature(&(altimeter -> bmp180));
    update_smooth_altitude(altimeter);
    update_smooth_velocity(altimeter);
}

/*
Utilizes a circular linked list to keep track of the most recent altitude readings.
This list is averaged to keep track of a 'smooth' altitude reading for state transitions.
*/
void update_smooth_altitude(struct Altimeter * altimeter){
    //"Value in the current node to equal the value the sensor just recorded"
    altimeter -> altitude_pointer -> value = altimeter -> bmp180.altitude;

    //Initializes sum variable and temporary pointer for traversing the loop
    float sum = 0;
    struct Node * loop_pointer = altimeter -> altitude_pointer;

    //1: Add current node to sum
    //2: Increment to next node
    //3: Stops after it reaches current altitude pointer
    do {
        sum += loop_pointer -> value;
        loop_pointer = loop_pointer -> next_address;
    } while(loop_pointer != altimeter-> altitude_pointer);
    
    //"The address of the new node is now equal to the address being pointed to"
    altimeter -> altitude_pointer = altimeter -> altitude_pointer -> next_address;
    altimeter -> smooth_altitude = sum/LINKED_LIST_SIZE;
}

void update_smooth_velocity(struct Altimeter * altimeter){
    altimeter -> smooth_velocity = 0.0;
}
