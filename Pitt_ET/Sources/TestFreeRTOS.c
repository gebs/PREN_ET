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
	  test = 1 + 1;
  }
}
