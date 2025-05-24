#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/i2c.h"
#include "pico/util/queue.h"

#include "header/altimeter.h"
#include "header/state_handler.h"
#include "header/data_logger.h"



uint8_t state = 1;
uint8_t * const state_pointer = &state;

// 'main' function for core 1
void core1_entry(){
    struct data_packet incoming_packet;


    while(1){
        if(state == 1){
            if(queue_is_full(&packet_queue)){
                queue_try_remove(&packet_queue,NULL);
            }
        }

        else{
            queue_remove_blocking(&packet_queue, (void*)&incoming_packet);
            log_data(&incoming_packet);
        }
        
    }
}


// core 0
int main()
{
    struct Altimeter altimeter;
    initialize_altimeter(&altimeter);


    initialize_data_logger();
    
    multicore_launch_core1(core1_entry);

    while(1){
        update_altimeter(&altimeter);
        handle_state(state_pointer,&altimeter);

        make_packet(&altimeter, state);
        queue_try_add(&packet_queue,(void*)&packet);

    }

    return 0;
}
