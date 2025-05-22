#include "header/data_logger.h"

FATFS filesystem; // FileSystem
FIL file; // File

void initialize_data_logger(char file_name[]){
    f_mount(&filesystem,"",1);

    const char* const filename = file_name;

    f_open(&file,filename, FA_OPEN_APPEND | FA_WRITE);
}

void log_data(struct data_packet* data_packet){
	f_printf(&file,"log\n");
}

void end_logging(){
	f_close(&file);
	f_unmount("");
}