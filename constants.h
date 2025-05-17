#ifndef CONSTANTS_H
#define CONSTANTS_H

//State bits
#define STANDBY 1
#define BOOST 2
#define COAST 4
#define FREEFALL 8
#define LANDED 16

//State change logic constants
#define TAKEOFF_HEIGHT_THRESHOLD 10.0
#define TAKEOFF_VELO_THRESHOLD 5.0
#define MOTOR_BURNOUT_PERCENT 0.4
#define FREEFALL_VELOCITY_THRESHOLD -5.0
#define LANDED_VELOCITY_THRESHOLD -5.0
#define LANDED_HEIGHT_THRESHOLD 5.0



#define LINKED_LIST_SIZE 10

//BMP180
#define I2C_PORT i2c0
#define I2C_SDA 0
#define I2C_SCL 1

#endif