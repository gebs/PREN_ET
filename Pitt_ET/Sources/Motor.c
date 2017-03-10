/*
 * Motor.c
 *
 *  Created on: Mar 3, 2017
 *      Author: Martina M
 */

#include "Motor.h"

void speedL(int dir, int speed)
{
	DIR_SetVal(DIR_DeviceData); // Richtung definieren
	PWM_R_SetDutyMS(speed);

}

void speedR(int dir, int speed)
{


}
