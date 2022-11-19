#include "ws2812.pio.h"                                                                 //Autogenrated library from Pioasm
#include "hardware/pio.h"                                                               //Chip specific Library

#define POWER_PIN 11                                                                    //power enable pin of neopixel
#define DATA_PIN 12                                                                     //signal pin of neopixel

#define PIO pio1                                                                        //PIO module used
#define STATE_MACHINE 0                                                                 //PIO state machine used
#define FREQ 800000                                                                     //Frequency of data communication with LED
#define IS_RGBW true                                                                    //RGB

void pixel_init(){

    gpio_init(POWER_PIN);                                                              //Initialize Power Pin
    gpio_set_dir(POWER_PIN,GPIO_OUT);                                                  //Set direction of Power Pin(Output)
    gpio_put(POWER_PIN,1);                                                             //Set Power pin high or 1

    uint offset = pio_add_program(PIO, &ws2812_program);                               //Add Pio program
    ws2812_program_init(PIO, STATE_MACHINE, offset, DATA_PIN , 800000, IS_RGBW);       //Setup State machine
}

void pixel_set(uint32_t rgb){
    uint grb = (rgb & 0xFF0000)>>8 | (rgb & 0x00FF00)<<8 | (rgb & 0x0000FF);           //Convert RGB to GRB
    pio_sm_put_blocking(PIO, 0, grb << 8u);                                            //Setting data to Neopixel
}