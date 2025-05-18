#include "header/altimeter.h"


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
    bmp_get_pressure_temperature(&(altimeter -> bmp180));

    //Creates circular linked list for easy traversal during data aquisition and averaging
    //Initializes all altitude with current altitude so zeroes do not affect velocity
    for(int i = 0; i<LINKED_LIST_SIZE-1; i++){
        altimeter -> altitude_readings[i].value = altimeter -> bmp180.altitude;
        altimeter -> altitude_readings[i].next_address = &(altimeter -> altitude_readings[i+1]);

        //Velocity assumed to be zero upon power on, will get updated to small number near zero
        //during the first iteration of calculations.
        altimeter -> velocity_calculations[i].value = 0;
        altimeter -> velocity_calculations[i].next_address = &(altimeter -> velocity_calculations[i+1]);

        altimeter -> altitude_readings[i].time = get_absolute_time();
    }    


    //Points tail of list to head, finalizing the circular list
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


void update_altimeter(struct Altimeter * altimeter){
    bmp_get_pressure_temperature(&(altimeter -> bmp180));
    //Order matters, altitude must be calculated before velocity for lagging pointer logic
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
    altimeter -> altitude_pointer -> time = get_absolute_time();

    //Initializes sum variable and temporary pointer for traversing the loop
    float sum = 0;
    struct Altitude_Node * loop_pointer = altimeter -> altitude_pointer;

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
    altimeter -> height = altimeter -> smooth_altitude - altimeter -> initial_altitude;
    altimeter -> max_height = (altimeter -> height)    *(altimeter -> height > altimeter -> max_height) +
                              (altimeter -> max_height)*(altimeter -> height <= altimeter -> max_height);
}

/*
    Average velocity is calculated over the length of the altitude
    readings list. After that, it is then put into its own linked list of previous 
    velocitiy readings, those are averaged to produce a smooth velocity reading.
    The lagging pointer hangs back to the newest value that the altimeter just recorded.
    At the end of update_smooth_altitude, the altitude pointer moves on to the oldest value
    in the list, waiting to update it in the next iteration. By lagging the lagging pointer behind,
    we now have easy access to the newest and oldest value for height calculations.
*/
void update_smooth_velocity(struct Altimeter * altimeter){

    // ToDo: figure out how to get accurate DT from oldest to newest value
    double dt = absolute_time_diff_us(altimeter -> lagging_pointer -> time, altimeter -> altitude_pointer -> time)*0.000001;
    float height_difference =  (altimeter -> altitude_pointer -> value) - (altimeter -> lagging_pointer -> value);
    
    //Lagging pointer catches back up to altitude pointer, but will lag behind in update_smooth_altitude
    altimeter -> lagging_pointer = altimeter -> altitude_pointer;
    altimeter -> velocity_pointer -> value = height_difference / (LINKED_LIST_SIZE * dt);

    float sum = 0;
    struct Velocity_Node * loop_pointer = altimeter -> velocity_pointer;
    
    do{
        sum += loop_pointer -> value;
        loop_pointer = loop_pointer -> next_address;
    } while(loop_pointer != altimeter -> velocity_pointer);

    altimeter -> velocity_pointer = altimeter -> velocity_pointer -> next_address;
    altimeter -> smooth_velocity = sum/LINKED_LIST_SIZE;
    altimeter -> max_velocity = (altimeter -> smooth_velocity) * (altimeter -> smooth_velocity > altimeter -> max_velocity) + 
                                (altimeter -> max_velocity)    * (altimeter -> smooth_velocity <= altimeter -> max_velocity);
}


