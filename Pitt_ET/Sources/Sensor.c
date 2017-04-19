/*
 * Sensor.c
 *
 *  Created on: Apr 18, 2017
 *      Author: famga
 */
#include "Sensor.h"

SemaphoreHandle_t xSemaphore;

 void Sensor(void *pvParameters) {
  (void)pvParameters; /* parameter not used */
  
  TickType_t xLastWakeTime;
  TickType_t xFrequency = 1000;
  xLastWakeTime = xTaskGetTickCount();
  US_Init();
  
  int i= 0; 
  for(;;) {
	  
	 /* Term1_SendNum(i); 
	  Term1_SendStr(". Durchlauf       "); 
	   WAIT1_Waitms(50); 
	  FRTOS1_vTaskDelayUntil(&xLastWakeTime,xFrequency/portTICK_RATE_MS); 	// wait milisec
		i++; 			
	 */
	  FRTOS1_vTaskDelayUntil(&xLastWakeTime,xFrequency/portTICK_RATE_MS); 	// wait milisec
	  	Measure(); 
  }
}

 typedef enum {
   ECHO_IDLE, /* device not used */
   ECHO_TRIGGERED, /* started trigger pulse */
   ECHO_MEASURE, /* measuring echo pulse */
   ECHO_OVERFLOW, /* measurement took too long */
   ECHO_FINISHED /* measurement finished */
 } US_EchoState;

 
 
 typedef struct US_DeviceType{
   LDD_TDeviceData *trigDevice; /* device handle for the Trigger pin */
   LDD_TDeviceData *echoDevice; /* input capture device handle (echo pin) */
   volatile US_EchoState state; /* state */
   TU1_TValueType capture; /* input capture value */
   void (*setVal)(LDD_TDeviceData*); /**< function pointer for TRIG device */
   void (*clrVal)(LDD_TDeviceData*); /**< function pointer for TRIG device */
 } US_DeviceType;


 
 typedef struct US_Devices {

 	US_DeviceType usDevice[5];
 	uint8_t currentDevice;
 } US_Devices;

 US_Devices usDevices; 



 
 void US_EventEchoOverflow(LDD_TUserData *UserDataPtr) {
 	US_Devices *ptr; 
 	ptr = (US_Devices*)UserDataPtr; 
 	ptr -> usDevice[ptr->currentDevice].state = ECHO_OVERFLOW;  
 }

 
 
 void US_EventEchoCapture(LDD_TUserData *UserDataPtr) {
     US_Devices *ptr; 
     ptr = (US_Devices*)UserDataPtr;
 	if (ptr->usDevice[ptr->currentDevice].state==ECHO_TRIGGERED){
 		 TU2_ResetCounter(ptr->usDevice[ptr->currentDevice].echoDevice); 
 		 ptr->usDevice[ptr->currentDevice].state = ECHO_MEASURE; 
 	}
 	else if (ptr->usDevice[ptr->currentDevice].state==ECHO_MEASURE){
 		 (void)TU2_GetCaptureValue(ptr->usDevice[ptr->currentDevice].echoDevice, ptr->currentDevice, &ptr->usDevice[ptr->currentDevice].capture);
 		 ptr->usDevice[ptr->currentDevice].state = ECHO_FINISHED;
 	}

 }

 

  uint16_t calcAirspeed_dms(uint8_t temperatureCelsius){ 

   /* Return the airspeed depending on the temperature, in deci-meter per second */
   unsigned int airspeed; /* decimeters per second */

   airspeed = 3313 + (6 * temperatureCelsius); /* dry air, 0% humidity, see http://en.wikipedia.org/wiki/Speed_of_sound */
   airspeed -= (airspeed/100)*17; /* factor in a relative humidity of ~40% */ 
   return airspeed;
 }

 uint16_t US_usToMillimeters(uint16_t microseconds, uint8_t temperatureCelsius) {
   return (microseconds*1000UL)/calcAirspeed_dms(temperatureCelsius)/2; /* 2 because of two way */
 }
 

 /* measure and return the microseconds */
 uint16_t US_Measure_us(uint8_t i) {
   uint16_t us;
   TickType_t xLastWakeTime;
     TickType_t xFrequency = 10;
     xLastWakeTime = xTaskGetTickCount();
   /* send 10us pulse on TRIG line. */
   
     
     
     usDevices.currentDevice = i; 
   usDevices.usDevice[i].setVal(usDevices.usDevice[i].trigDevice);
   WAIT1_Waitus(10);
  
   
 	usDevices.usDevice[i].state = ECHO_TRIGGERED;
 	usDevices.usDevice[i].clrVal(usDevices.usDevice[i].trigDevice);
 	

 	while(usDevices.usDevice[i].state!=ECHO_FINISHED)
 	{
 		/* measure echo pulse */
 		if (usDevices.usDevice[i].state==ECHO_OVERFLOW)
 		{
 			/* measurement took too long? */
 			usDevices.usDevice[i].state = ECHO_IDLE; 
 			return 0; }
 	}
 	us =  (usDevices.usDevice[i].capture*1000UL)/(TU2_CNT_INP_FREQ_U_0/1000);
     return us; 
 }



 void US_Init(void) {
 	usDevices.usDevice[0].state = ECHO_IDLE; 
 	usDevices.usDevice[0].capture = 0; 
 	usDevices.usDevice[0].trigDevice = TRIG_Init(NULL); 
 	usDevices.usDevice[0].echoDevice = TU2_Init(&usDevices);
 	usDevices.usDevice[0].clrVal = TRIG_ClrVal;
 	usDevices.usDevice[0].setVal = TRIG_SetVal;
 	
 	usDevices.usDevice[1].state = ECHO_IDLE; 
 	usDevices.usDevice[1].capture = 0; 
 	usDevices.usDevice[1].trigDevice = TRIG2_Init(NULL); 
 	usDevices.usDevice[1].echoDevice = usDevices.usDevice[0].echoDevice; 
 	usDevices.usDevice[1].clrVal = TRIG2_ClrVal;
 	usDevices.usDevice[1].setVal = TRIG2_SetVal;

 	usDevices.usDevice[2].state = ECHO_IDLE; 
 	usDevices.usDevice[2].capture = 0; 
 	usDevices.usDevice[2].trigDevice = TRIG3_Init(NULL); 
 	usDevices.usDevice[2].echoDevice = usDevices.usDevice[0].echoDevice; 
 	usDevices.usDevice[2].clrVal = TRIG3_ClrVal;
 	usDevices.usDevice[2].setVal = TRIG3_SetVal;
 	
 	usDevices.usDevice[3].state = ECHO_IDLE; 
 	usDevices.usDevice[3].capture = 0; 
 	usDevices.usDevice[3].trigDevice = TRIG4_Init(NULL); 
 	usDevices.usDevice[3].echoDevice = usDevices.usDevice[0].echoDevice; 
 	usDevices.usDevice[3].clrVal = TRIG4_ClrVal;
 	usDevices.usDevice[3].setVal = TRIG4_SetVal;
 	
 	usDevices.usDevice[4].state = ECHO_IDLE; 
 	usDevices.usDevice[4].capture = 0; 
 	usDevices.usDevice[4].trigDevice = TRIG5_Init(NULL); 
 	usDevices.usDevice[4].echoDevice = usDevices.usDevice[0].echoDevice; 
 	usDevices.usDevice[4].clrVal = TRIG5_ClrVal;
 	usDevices.usDevice[4].setVal = TRIG5_SetVal;
 	
 }
 
  void Measure(void) {
   uint8_t i = 0;
   uint16_t us= 0, cm= 0;
   uint8_t buf[8];
  

 // Term1_Cls(); 
  Term1_CursorDown(1); 
  Term1_CursorLeft(80);
   
  for(i;i<5;i++){
   
   us = US_Measure_us(i); 
   
   UTIL1_Num16uToStrFormatted(buf, sizeof(buf), us, ' ', 5);
     
   cm = US_usToMillimeters(us, 22);
   UTIL1_Num16uToStrFormatted(buf, sizeof(buf), cm, ' ', 5);
   Term1_SendStr("   "); 
   Term1_SendNum(cm); 
   Term1_SendStr("mm       "); 
   WAIT1_Waitms(50); 
   
   }
 
  

 	}
