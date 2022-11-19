#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "string.h"
#include "pio_i2c.h"
//#include "pixel.h"
//#include "pico/binary_info.h"

#define ENABLE_REG 0x80
#define PDATA_REG  0x9C
#define CDATA_REG_L 0x94
#define ALS_TIME_REG 0x81

#define proximity_en 1
#define color_sensor_en 1
#define gesture_en 0

#define ALS_TIME 0

const int addr = 0x39;
const int max_read = 250;


#define SDA 22
#define SCL 23

void apds_init(){
    uint offset = pio_add_program(pio1, &i2c_program);
    i2c_program_init(pio1, 0, offset, SDA, SCL);
    uint8_t buf[2];
    buf[0] = ENABLE_REG; //the register which hold all the enable bits
    buf[1] = (proximity_en << 2u) | (color_sensor_en << 1u) | (gesture_en << 6u) | 1; //Shifting bits to respective enable positions in the byte
    pio_i2c_write_blocking(pio0, 0, addr, buf, 2, false);//send address of the register and the value

    buf[0] = ALS_TIME_REG;//Register address of the color sensor
    buf[1] = ALS_TIME;//register value to be set
    pio_i2c_write_blocking(pio0, 0, addr, buf, 2, false);//sending the command
}

void proximity_read(int32_t *proximity){
    uint8_t buf[0];
    uint8_t reg = PDATA_REG;//register address which holds the proximity data
    pio_i2c_write_blocking(pio0, 0, addr, &reg, 1, true);  //writing the address to be read
    pio_i2c_read_blocking(pio0, 0, addr, buf, 1, false);  //reading the address
    *proximity = buf[0];
}

void color_read(int32_t *r, int32_t *g, int32_t *b,int32_t *c){
    uint8_t buf[8];
    uint8_t reg = CDATA_REG_L;
    pio_i2c_write_blocking(pio0, 0, addr, &reg, 1, true);  // true to keep master control of bus
    pio_i2c_read_blocking(pio0, 0, addr, buf, 8, false);  // false - finished with bus

    *c = (buf[1] << 8) | buf[0];
    *r = (buf[3] << 8) | buf[2];
    *g = (buf[5] << 8) | buf[4];
    *b = (buf[7] << 8) | buf[6];
}

int main() {
    stdio_init_all();
    pixel_init();

    uint offset = pio_add_program(pio0, &i2c_program);
    i2c_program_init(pio0, 0, offset, SDA, SCL);

        sleep_ms(5000);

    apds_init();
    printf("Hello, APDS9960! Reading raw data from module...\n");

    while (true) {
        
        int32_t proximity;
        int32_t r,g,b,c;

        proximity_read(&proximity);
        color_read(&r, &g, &b, &c);

        printf("proximity: %d   ",proximity);
        printf("r:%d, g:%d, b:%d, c:%d\n", r, g, b, c);
        //pixel_set(r<<16 | g<<8 | b);

        // Wait for data to refresh
        //sleep_ms(1000);

    }
}