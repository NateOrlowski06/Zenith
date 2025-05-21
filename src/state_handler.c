#include "header/state_handler.h"
#include <time.h>

/*

    'Branchless' statements are employed in all state logic in 
    an effort to optimize the amount of processing done in program execution

    An explanation of branchless programming can be found here:
    https://dev.to/jobinrjohnson/branchless-programming-does-it-really-matter-20j4

    The handle_state funciton is called each loop iteration and determines which state needs to be handled

    There are 5 stages of a rocket launch; standby, boost, coast, freefall, and landed. 
    For each of these states, there could be a variety of different tasks that need not be done in other states

    The state variable is stored in a 1 byte integer and is kept as a single bit
    00000001 is standby, 00000010 is boost, ...

    Each state handler function can evaluate whether that bit needs to be shifted to the next state or not

*/




// Array of pointers to function that are responsible for handling the current state
void (*state_handlers[5])(uint8_t *state, struct Altimeter* altimeter) = {handle_standby,
                                                                          handle_boost,
                                                                          handle_coast,
                                                                          handle_freefall,
                                                                          handle_landed};

/*

    Called ever loop iteration by main.c and passed a pointer to state and pointer to altimeter struct
    Calls the correct function to handle state by gathering the number of leading binary zeros in *state

*/
inline void handle_state(uint8_t *state, struct Altimeter* altimeter){
    state_handlers[__builtin_ctz(*state)](state, altimeter);
}

/*

    Called by handle_state, constantly checks for launch conditions
    
    Standby ends when the velocity and height are simultaneously above their thresholds for launch

*/
void handle_standby(uint8_t *state, struct Altimeter* altimeter){

    // TODO: deadband height while rocket is still on the pad

    // State shifted to the left 1 if conditions are met. 
    *state = *state << (altimeter -> height > TAKEOFF_HEIGHT_THRESHOLD && 
               altimeter -> smooth_velocity > TAKEOFF_VELO_THRESHOLD);
}

/*

    The rocket is considered to be coasting when the motor burns out
    This function evaluates whether the motor is still burning or not

    In a rockets flight, the maximum velocity is achieved at the very end of motor burn

    Boost ends when the current velocity is substantially less than the maximum velocity
    A percentage of max_velocity is used to ensure motor burnout is not detected too early

*/
void handle_boost(uint8_t *state, struct Altimeter* altimeter){

    // Branchless statement
    *state = *state << ((altimeter -> max_velocity * MOTOR_BURNOUT_PERCENT) > (altimeter -> smooth_velocity));
}

/*

    In the condition that launch has been falsely detected by a pressure spike,
    this funciton has the option to send the state back to standby before arming the charges

    Coast ends when the rockets reaches apogee, velocity is less than a threshold and the height is less than the maximum height

*/
void handle_coast(uint8_t *state, struct Altimeter* altimeter){
    // Moves the state back to standby if the altitude is less than expected by coasting state
    if(altimeter -> smooth_altitude < SAFE_ARMING_HEIGHT){
        *state = *state >> 2;
        return;
    }
    altimeter -> is_armed = ARM;

    // Branchless statement
    *state = *state << ((altimeter -> max_height > altimeter -> height) && (altimeter -> smooth_velocity < FREEFALL_VELOCITY_THRESHOLD));
}

/*

    Handles deployment of parachutes

    Without blocking the rest of the program's execution, the gpio pin needs to be 
    set to high for a specified 'pulse duration' to fire the ignitor (LED in my case)

    Freefall has ended whenever the rocket is below a height threshold and above a velocity threshold which is a negative number

*/
void handle_freefall(uint8_t *state, struct Altimeter* altimeter){
    // Freefall_start initializied to zero statically, updated on first funciton call
    static absolute_time_t freefall_start_time = 0;
    static absolute_time_t main_deployment_time = 0;


    // This value is only ever added to the instance freefall_start_time = 0, otherwise, zero is added to it. Branchless
    freefall_start_time += (freefall_start_time == 0) * get_absolute_time();


    // GPIO is set to on if it has been less than 2 seconds from freefall detection and altimeter is armed
    gpio_put(DROGUE_CHARGE_PIN, (absolute_time_diff_us(freefall_start_time, get_absolute_time()) < PULSE_DURATION * US_TO_SEC) && 
                                 altimeter -> is_armed);


    #ifdef DUAL_DEPLOY
    
    // Branchless
    main_deployment_time += get_absolute_time()* ((main_deployment_time == 0) && (altimeter -> height < MAIN_DEPLOYMENT_HEIGHT));

    int main_deployment_condition =   (absolute_time_diff_us(main_deployment_time, get_absolute_time()) < PULSE_DURATION * US_TO_SEC) && 
                                      (altimeter -> height < MAIN_DEPLOYMENT_HEIGHT) && 
                                      (altimeter -> is_armed);
    
    gpio_put(MAIN_CHARGE_PIN, main_deployment_condition);
    #endif

    // Branchless statement
    *state = *state << ((altimeter -> height < LANDED_HEIGHT_THRESHOLD) && ((altimeter -> smooth_velocity) > LANDED_VELOCITY_THRESHOLD));
}

/*

    Ensures neither gpio pin is left on high
    disarms the rocket to ensure the pins cannot return to high

*/
void handle_landed(uint8_t *state, struct Altimeter* altimeter){
    gpio_put(DROGUE_CHARGE_PIN,0);
    gpio_put(MAIN_CHARGE_PIN,0);
    altimeter -> is_armed = DISARM;
}

