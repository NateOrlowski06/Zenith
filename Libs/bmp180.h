#ifndef BMP180_H
#define BMP180_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define BMP_REG_CONTROL         0xF4
#define BMP_REG_RESULT          0xF6
#define BMP_COM_TEMP            0x2E
#define BMP_COM_PRES            0x34
#define BMP_CALIB_COEFF_LEN     0x16
#define BMP_CALIB_COEFF_REG     0xAA
#define BMP_CHIP_ID_REG         0xD0
#define BMP_CHIP_ID_VAL         0x55
#define BMP_MIN_TEMP_THRESHOLD	-40
#define BMP_MAX_TEMP_THRESHOLD	+85
#define BMP_TEMP_DELAY          5

#define ASSERT_OK(X) { if (X == false) return false; };

typedef struct {
    int16_t  AC1;
    int16_t  AC2;
    int16_t  AC3;
    uint16_t AC4;
    uint16_t AC5;
    uint16_t AC6;
    int16_t  B1;
    int16_t  B2;
    int16_t  MB;
    int16_t  MC;
    int16_t  MD;
} bmp_calib_param_t;

typedef struct {
    int addr;
    int rate;
    int scl;
    int sda;
    i2c_inst_t* inst;
} i2c_t;

typedef struct {
    i2c_t i2c;
    uint8_t oss;
    float temperature;
    int32_t pressure;
    float altitude;
    bmp_calib_param_t calib;
    int32_t B5;
} bmp_t;

extern const uint32_t oss_delay[];

bool bmp_check_chip_id(bmp_t* bmp);
bool bmp_get_calib_coeffs(bmp_t* bmp);
uint32_t bmp_start_temperature(bmp_t* bmp);
bool bmp_read_temperature(bmp_t* bmp);
uint32_t bmp_start_pressure(bmp_t* bmp);
bool bmp_read_pressure(bmp_t* bmp);
bool bmp_get_temperature(bmp_t* bmp);
bool bmp_get_pressure(bmp_t* bmp);
bool bmp_get_pressure_temperature(bmp_t* bmp);
bool bmp_init(bmp_t* bmp);

#endif