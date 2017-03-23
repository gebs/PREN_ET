/*
 * TestFreeRTOS.c
 *
 *  Created on: Mar 10, 2017
 *      Author: Martina M
 */
#include "TestFreeRTOS.h"
#include "speed_R.h"

TickType_t speedR;
TickType_t speedL;

void setSpeedR(uint8_t speed)
{
	speedR = speed;
}

void setSpeedL(uint8_t speed)
{
	speedL = speed;
}



 void Task1(void *pvParameters) {
  (void)pvParameters; /* parameter not used */
  
  TickType_t xLastWakeTime;
  TickType_t xFrequency = 1000;
  xLastWakeTime = xTaskGetTickCount();
  
  //FRTOS_vTaskDelay(10/portTICK_RATE_MS); //super codezeile von michi
  
  LDD_TDeviceData *speed_R_DeviceData; // needed for nothing
  for(;;) {
 // do something
	  xFrequency = speedR;
	  
	  FRTOS1_vTaskDelayUntil(&xLastWakeTime,xFrequency/portTICK_RATE_MS); 	// wait milisec
	  speed_R_SetVal(speed_R_DeviceData);									// pos Flanke
	  
	  xFrequency = 10;														
	  
	  FRTOS1_vTaskDelayUntil(&xLastWakeTime,xFrequency);					// wait short
	  speed_R_ClrVal(speed_R_DeviceData);									// neg Flanke
	  
	  DIR_R_ClrVal(DIR_R_DeviceData); // Richtung definieren 0
	  //DIR_R_ClrVal(DIR_R_DeviceData); // Richtung definieren 0
  }
}

 
 void Motor(void *pvParameters) {
  (void)pvParameters; /* parameter not used */
  int test;
  for(;;) {
	 // DIR_SetVal(DIR_DeviceData);
	  //WAIT1_Waitms(500);
	 // DIR_ClrVal(DIR_DeviceData);
  }
}
