/*
 * main.c
 *
 *  Created on: Oct 17, 2022
 *      Author: Eng AFS
 */
#include "ultrasonic.h"
#include "lcd.h"
#include <avr/io.h>
#include "gpio.h"


int main(void)
{
	uint16 ultrasonic_distance = 0;
	LCD_init();
	Ultrasonic_init();
	SREG|=(1<<7);
	LCD_displayString("Distance =    cm");

	while(1)
	{
		ultrasonic_distance = Ultrasonic_readDistance();
		LCD_moveCursor(0,11);
		if (ultrasonic_distance>99)
				{
					LCD_intgerToString(ultrasonic_distance);
				}
				else {
					LCD_intgerToString(ultrasonic_distance);
					LCD_displayCharacter(' ');
				}
			}
	}


