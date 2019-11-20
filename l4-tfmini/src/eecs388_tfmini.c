/**
 * This program read the distance data from TFmini lidar sensor and 
 * print the read data and controls LED depending on the measured distance.
 */
#include <stdint.h>
#include <stdio.h>

#include "eecs388_lib.h"

int main()
{
    gpio_mode(RED_LED, OUTPUT);
    gpio_mode(GREEN_LED, OUTPUT);
    ser_setup();

    ser_printline("Setup completed.\n");

    int dist; // read distance value. 

    while (1) {
        /* 
            Task 1: 
            - read a data frame from the TFmini sensor
            - print the read distance data. e.g., "dist: 45 cm"
              (you can use either printf or sprintf & ser_printline function)
        */
        if ('Y' == ser_read() && 'Y' == ser_read()) {
            //store the Dist_L and Dist_H in a variable by ser_read()
            int16_t d1 = ser_read();
            int16_t d2 = ser_read();
            //formula to make the distance into a 16 bit
            dist = (d2 << 8) | d1;
            printf("distance is %d",dist);

            // ser_read() 5 times to read data frame that is not used in lab
            ser_read();
            ser_read();
            ser_read();
            ser_read();
            ser_read();
            
        }
        /* 
            Task 2: 
            - turn on the red light if the distance is less than 50cm. 
            - otherwise turn on the green light 
        */
        if (dist < 50) {
            //closing the GREEN_LED and opening the RED_LED
            gpio_write(RED_LED,ON);
            gpio_write(GREEN_LED,OFF);
        } else {
            //closing the RED_LED and opening the GREEN_LED
            gpio_write(RED_LED,OFF);
            gpio_write(GREEN_LED,ON);
        }
    }
}