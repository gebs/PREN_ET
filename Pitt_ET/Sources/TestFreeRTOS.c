/*
 * TestFreeRTOS.c
 *
 *  Created on: Mar 10, 2017
 *      Author: Martina M
 */
#include "TestFreeRTOS.h"
#include "speed_R.h"
#include "LED1.h"

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



 void MotorR(void *pvParameters) {
  (void)pvParameters; /* parameter not used */
  
  TickType_t xLastWakeTime;
  TickType_t xFrequency = 1000;
  xLastWakeTime = xTaskGetTickCount();
  
  
  //LDD_TDeviceData *speed_R_DeviceData; // needed for nothing
  for(;;) {
 // do something
	  xFrequency = speedR;
	  xFrequency = 800; //Test Wert
	  FRTOS1_vTaskDelayUntil(&xLastWakeTime,xFrequency/portTICK_RATE_MS); 	// wait milisec
	  speed_R_SetVal(speed_R_DeviceData);									// pos Flanke
	  
	  xFrequency = 10;														
	  xFrequency = 400; //Test Wert
	  FRTOS1_vTaskDelayUntil(&xLastWakeTime,xFrequency/portTICK_RATE_MS);	// wait short
	  speed_R_ClrVal(speed_R_DeviceData);									// neg Flanke
	  
	  DIR_R_ClrVal(DIR_R_DeviceData); // Richtung definieren 0
	  
  }
}

 
 void Motor(void *pvParameters) {
  (void)pvParameters; /* parameter not used */
  
  TickType_t xLastWakeTime;
  TickType_t xFrequency = 1000;
  xLastWakeTime = xTaskGetTickCount();
  
  
  for(;;) {

	  xFrequency = 400; //Test Wert
	  FRTOS1_vTaskDelayUntil(&xLastWakeTime,xFrequency/portTICK_RATE_MS); 	// wait milisec
					
	  LED1_Neg(); // für Testzweck
	  												
	  xFrequency = 200; //Test Wert
	  FRTOS1_vTaskDelayUntil(&xLastWakeTime,xFrequency/portTICK_RATE_MS);	// wait short
	  									// neg Flanke
	  LED1_Neg(); // für Testzweck
  }
}
