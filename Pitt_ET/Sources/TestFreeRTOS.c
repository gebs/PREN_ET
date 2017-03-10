/*
 * TestFreeRTOS.c
 *
 *  Created on: Mar 10, 2017
 *      Author: Martina M
 */
#include "TestFreeRTOS.h"



 void Task1(void *pvParameters) {
  (void)pvParameters; /* parameter not used */
  int test;
  for(;;) {
 // do something
	 // test = 1 + 1;
	  Test_SetVal(Test_DeviceData);
	  WAIT1_Waitms(500);
	  Test_ClrVal(Test_DeviceData);
  }
}

 
 void Motor(void *pvParameters) {
  (void)pvParameters; /* parameter not used */
  int test;
  for(;;) {
	  DIR_SetVal(DIR_DeviceData);
	  WAIT1_Waitms(500);
	  DIR_ClrVal(DIR_DeviceData);
  }
}
