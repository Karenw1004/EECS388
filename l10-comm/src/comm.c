#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "eecs388_lib.h"

int main()
{
    // initialize UART channels
    ser_setup(0); // uart0 (debug)
    ser_setup(1); // uart1 (raspberry pi)

    printf("Setup completed.\n");
    printf("Begin the main loop.\n");

    int prev_dist = 0;
    int prev_angle = 0; 

    //Lab2: Blinking Led
    //declare array
    int ledArr[3];
    //value of the array 
    ledArr[0] = GREEN_LED;
    ledArr[1] = RED_LED ;
    ledArr[2] = BLUE_LED;

    for (int i = 0 ; i < 3 ; i++){
        gpio_mode(ledArr[i], OUTPUT);
    }

    while (1) {

        // Receive the data(steering angle) from pi 4 channel
        if ( ser_isready(1) ){
            double data = ser_read(1);
            //Lab5: servo motor control
            int gpio = PIN_19;
            gpio_mode(gpio, OUTPUT); 

            //Lab4 : TfLidar
            //read a data frame from the TFmini sensor
            if ('Y' == ser_read() && 'Y' == ser_read()) {
                //store the Dist_L and Dist_H in a variable by ser_read()
                int16_t d1 = ser_read();
                int16_t d2 = ser_read();
                //formula to make the distance into a 16 bit
                dist = (d2 << 8) | d1;
                printf("distance is %d",dist);
                
                for (int i = 0 ; i < 5 ; i++){
                // ser_read() 5 times to read data frame that is not used in lab
                ser_read();
                }               
            }
            //turn on the red light if the distance is less than 50cm
            if (dist < 50) {
                allLeds(OFF);
                gpio_write(ledArr[1],ON); // red ==stop  
                break;
            }

            printf("pos: %d (degree)\n", pos);
            /* control the servor for 1 sec duration */
            if (pos < (-30) ) {
                allLeds(OFF);
                gpio_write(gpio[2], ON); //blue==left
            } else if (pos > (30)) {
                allLeds(OFF);
                gpio_write(gpio[0], ON);//green==right
            } else {
                allLeds(ON);//green==right
            }           
        }
    }
}
void allLeds(int mode ){
    gpio_write(gpio[0], mode);
    gpio_write(gpio[1], mode);
    gpio_write(gpio[2], mode);
}

//Generate a pwm signal , @gpio   gpio number, @pos    degree [0,180]
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

