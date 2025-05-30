#ifndef CONSTANTS_H
#define CONSTANTS_H


#define ARM 1
#define DISARM 0

//State change logic constants
//TODO: refine these values
#define TAKEOFF_HEIGHT_THRESHOLD 5.0
#define TAKEOFF_VELO_THRESHOLD 5.0
#define MOTOR_BURNOUT_PERCENT 0.95
#define FREEFALL_VELOCITY_THRESHOLD -3.0
#define LANDED_VELOCITY_THRESHOLD -2.5
#define LANDED_HEIGHT_THRESHOLD 5.0
#define APOGEE_DETECT_THRESHOLD 3.0


#define LINKED_LIST_SIZE 5

//BMP180
#define I2C_PORT i2c1
#define I2C_SDA 26
#define I2C_SCL 27

#define DROGUE_CHARGE_PIN 3
#define MAIN_CHARGE_PIN 2
#define PULSE_DURATION 2

#define SAFE_ARMING_HEIGHT 50.0

#define MAIN_DEPLOYMENT_HEIGHT 250.0

#define US_TO_SEC 1000000.0

//Comment this line for single deploy at apogee
//#define DUAL_DEPLOY

// Pins for sd card reader
#define RX 16
#define TX 19
#define CSN 17
#define SCK 18
#define SPI_PORT SPI0

#define DATA_PACKETS_QUEUE_SIZE 10

#endif