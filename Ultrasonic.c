/*
 * Ultrasonic.c
 *
 *  Created on: Oct 15, 2022
 *      Author: Eng AFS
 */

#include "Ultrasonic.h"
#include "gpio.h"
#include "common_macros.h"
#include "ICU.h"
#include <util/delay.h>

Icu_ConfigType ICU={
		F_CPU_8,
		RISING,
};
uint8 edgecount;
uint16 time = 0;

/*
 * Setup the direction for the trigger pin as output pin through the GPIO driver.
 * Setup the ICU call back function.
 * Initialize the ICU driver as required.
 */
void Ultrasonic_init(void)
{
	GPIO_setupPinDirection(PORTD_ID,PIN6_ID,PIN_INPUT); ///ECHO
	GPIO_setupPinDirection(PORTB_ID,PIN5_ID,PIN_OUTPUT); /// trigger
	GPIO_writePin(PORTB_ID,PIN5_ID,LOGIC_LOW);
	Icu_init(&ICU);
	Icu_setCallBack(Ultrasonic_edgeProcessing);

}

/*
 * Send the Trigger pulse to the ultrasonic.
 */
void Ultrasonic_Trigger(void)
{
	GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_HIGH);
	_delay_us(10);
	GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_LOW);
}

/*
 * Send the trigger pulse by using Ultrasonic_Trigger function.
 * Start the measurements by the ICU from this moment.
 */
uint16 Ultrasonic_readDistance(void)
{
	Ultrasonic_Trigger();
	return ((float32)time/(58.8));
}
/*
 * This is the call back function called by the ICU driver.
 * This is used to calculate the high time (pulse time) generated by the ultrasonic sensor.
 */
void Ultrasonic_edgeProcessing(void)
{
	edgecount++;
			if(edgecount == 1)
			{
				/*
				 * Clear the timer counter register to start measurements from the
				 * first detected rising edge
				 */
				Icu_clearTimerValue();
				/* Detect falling edge */
				Icu_setEdgeDetectionType(FALLING);
			}
			else if(edgecount == 2)
			{
				/* Store the High time value */
				time = Icu_getInputCaptureValue();
				/* Clear the timer counter register to start measurements again */
				Icu_clearTimerValue();
				edgecount=0;
				/* Detect rising edge */
				Icu_setEdgeDetectionType(RISING);
			}
}