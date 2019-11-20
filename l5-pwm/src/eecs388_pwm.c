#include <stdio.h>
#include <stdint.h>

#include "eecs388_lib.h"

#define SERVO_PULSE_MAX 2400    /* 2400 us */
#define SERVO_PULSE_MIN 544     /* 544 us */
#define SERVO_PERIOD    20000   /* 20000 us (20ms) */

/**
 * Generate a pwm signal
 * 
 * Input:
 *  @gpio   gpio number
 *  @pos    degree [0,180]
 */ 
void servo(int gpio, int pos)
{
    //the formula of pulse
    int pulse = (SERVO_PULSE_MIN + ( (SERVO_PULSE_MAX - SERVO_PULSE_MIN) /180)*pos);
    //switching on the PIN_19 for a pulse amount of time
    gpio_write(gpio,ON);
    delay_usec(pulse);
    //switching off the PIN_19 for a SERVO_PERIOD - pulse amount of time
    gpio_write(gpio,OFF);
    delay_usec(SERVO_PERIOD - pulse);
}

int main()
{
    int gpio = PIN_19;
    gpio_mode(gpio, OUTPUT);

    while (1) {
        for (int pos = 0; pos <= 180; pos += 30) {
            printf("pos: %d (degree)\n", pos);
            /* control the servor for 1 sec duration */
            for (int i = 0; i < 50; i++)
                servo(gpio, pos);
        }
    }
}