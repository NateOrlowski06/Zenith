
#include "Libs/bmp180.h"

#define LINKED_LIST_SIZE 10

#define I2C_PORT i2c0
#define I2C_SDA 0
#define I2C_SCL 1

struct Node{
    float value;
    struct Node * next_address;
};

struct Altimeter{
    bmp_t bmp180;
    struct Node altitude_readings[LINKED_LIST_SIZE];
    struct Node velocity_calculations[LINKED_LIST_SIZE];
    float smooth_altitude;
    float smooth_velocity;
    struct Node * altitude_pointer;
    struct Node * velocity_pointer;
};


void update_altimeter(struct Altimeter * altimeter);

void update_smooth_altitude(struct Altimeter * altimeter);
void update_smooth_velocity(struct Altimeter * altimeter);


void initialize_altimeter(struct Altimeter * altimeter);
