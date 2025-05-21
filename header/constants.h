#ifndef CONSTANTS_H
#define CONSTANTS_H

//State bits
#define STANDBY 1
#define BOOST 2
#define COAST 4
#define FREEFALL 8
#define LANDED 16

#define ARM 1
#define DISARM 0

//State change logic constants
//TODO: refine these values
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

#define DROGUE_CHARGE_PIN 20
#define MAIN_CHARGE_PIN 21
#define PULSE_DURATION 2

#define SAFE_ARMING_HEIGHT 50.0

#define MAIN_DEPLOYMENT_HEIGHT 100.0

#define US_TO_SEC 1000000.0

//Comment this line for single deploy at apogee
#define DUAL_DEPLOY

// Pins for sd card reader
#define RX 16
#define TX 19
#define CSN 17
#define SCK 18
#define SPI_PORT SPI0

#endif