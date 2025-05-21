#include "header/data_logger.h"

FATFS filesystem; // FileSystem
FIL file; // File

void initialize_data_logger(char file_name[]){
    f_mount(&filesystem,"",1);

    const char* const filename = file_name;

    f_open(&file,filename, FA_OPEN_APPEND | FA_WRITE);
}

void log_data(char data[],int size){
	for(int i = 0; i<size; i++){
		f_printf(&file,"test");
	}
	f_printf(&file,"\n");
}

void end_logging(){
	f_close(&file);
	f_unmount("");
}