#include "header/data_logger.h"

FATFS filesystem; // FileSystem
FIL file; // File

void initialize_data_logger(){
    f_mount(&filesystem,"",1);

    const char* const filename = "flight.csv";
    f_open(&file,filename, FA_OPEN_APPEND | FA_WRITE);
    f_printf(&file,"Timestamp,raw_pressure,temperature,raw_altitude,smooth_altitude,height,raw_velocity,smooth_velocity,is_armed,state\n");
}

void log_data(struct data_packet* data_packet){
	f_printf(&file,"%lld,%lld,%f,%f,%f,%f,%f,%f,%d,%d\n",
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

char* get_file_name(){
	int i = 1;
	FRESULT res;
	FILINFO fno;
	static char name[50];


	while(1){
		snprintf(name, sizeof(name),"flight%d.csv",i);
		res = f_stat(name, &fno);
		if(res != FR_OK){
			break;
		}
		i++;
	}
	return name;
}

void end_logging(){
	f_close(&file);
	f_unmount("");
}