#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H




#include "hw_config.h"
#include "f_util.h"
#include "ff.h"





void initialize_data_logger(char[]);
void log_data(char[],int);
void end_logging();

#endif

