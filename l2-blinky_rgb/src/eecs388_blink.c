#include <stdint.h>

#include "eecs388_lib.h"

/*
  Task 2.1: Review eecs388_lib.h and eecs388_lib.c 

  Task 2.2: Implement RGB blinky. 

    - set red, green, and blue leds as output mode, using gpio_mode() function
    - in the main loop, turn on/off a different LED at each iteration. 
      for example, 
         at 1st iteration
            turn on RED led
	    delay 500 ms 
	    turn off RED led
	    delay 300 ms.
	 at 2nd iteration
	    do the same but for GREEN led
	 at 3rd iteration
	    do the same but for BLUE led
	 at 4th iteration
	    back to RED led. 
	    ...
    - use array and modulo (%) operator to change the color. 

  Task 2.3. Implement RGBA (bonus: +1 point)
    - rotate red, green, blue, and white (all LEDs are enabled) colors.
*/

int main()
{
   //declare array
    int gpio[3];
   //value of the array 
    gpio[0] = GREEN_LED;
    gpio[1] = RED_LED ;
    gpio[2] = BLUE_LED;
    
   for (int i = 0 ; i < 3 ; i++){
    gpio_mode(gpio[i], OUTPUT);
   }
   
   while(1)
    { 
      for (int i = 0 ; i < 3 ; i++){
         gpio_write(gpio[i], ON);
         delay(500);
         gpio_write(gpio[i], OFF);
         delay(300);
         //if it reach the last index , next led is white
         if (i == 2){
            //to make it white, open all 3 led at the same time for 500ms
            gpio_write(gpio[0], ON);
            gpio_write(gpio[1], ON);
            gpio_write(gpio[2], ON);
            delay(500);
            //switch off the white led, keep for 300ms
            gpio_write(gpio[0], OFF);
            gpio_write(gpio[1], OFF);
            gpio_write(gpio[2], OFF);
            delay(300);
         }
      }
      
    }
}
