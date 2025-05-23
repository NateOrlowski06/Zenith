#include "header/data_logger.h"

FATFS filesystem; // FileSystem
FIL file; // File

struct data_packet packet;
queue_t packet_queue;
void initialize_data_logger(){

	queue_init_with_spinlock(&packet_queue, sizeof(struct data_packet), DATA_PACKETS_QUEUE_SIZE, 1);


    f_mount(&filesystem,"",1);

    const char* const filename = "flight.csv";
    f_open(&file,filename, FA_OPEN_APPEND | FA_WRITE);
    f_printf(&file,"Timestamp,Time,raw_pressure,temperature,raw_altitude,smooth_altitude,height,raw_velocity,smooth_velocity,is_armed,state\n");
}


void make_packet(struct Altimeter * altimeter, uint8_t state){

            packet.time = ((double)get_absolute_time() - (double)start_of_flight) / US_TO_SEC;
            packet.raw_pressure = altimeter -> bmp180.pressure;
            packet.temperature = altimeter -> bmp180.temperature;
            packet.raw_altitude = altimeter -> altitude_pointer -> value;
            packet.smooth_altitude = altimeter -> smooth_altitude;
            packet.height = altimeter -> height;
            packet.velocity = altimeter -> velocity_pointer -> value;
            packet.smooth_velocity = altimeter -> smooth_velocity;
            packet.is_armed = altimeter -> is_armed;
            packet.state = state;
        
}



void log_data(struct data_packet* data_packet){
	f_printf(&file,"%f,%.2f,%lld,%f,%f,%f,%f,%f,%f,%d,%d\n",
		data_packet -> time,
		data_packet -> time,
		data_packet -> raw_pressure,
		data_packet -> temperature,
		data_packet -> raw_altitude,
		data_packet -> smooth_altitude,
		data_packet -> height,
		data_packet -> velocity,
		data_packet -> smooth_velocity,
		data_packet -> is_armed,
		data_packet -> state
		);
	    f_sync(&file);

}



void end_logging(){
	f_close(&file);
	f_unmount("");
	queue_free(&packet_queue);
}