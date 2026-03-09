#ifndef __RTC_H
#define __RTC_H


#include "stm32f4xx_hal.h" 
#include "cmsis_os2.h"  
#include <stdio.h>
#include <string.h>

/* Defines related to Clock configuration */
#define RTC_ASYNCH_PREDIV  0x7F   /* LSE as RTC clock */
#define RTC_SYNCH_PREDIV   0x00FF /* LSE as RTC clock */

extern RTC_HandleTypeDef RtcHandle;
extern RTC_DateTypeDef sdatestructureget;
extern RTC_TimeTypeDef stimestructureget;
extern RTC_AlarmTypeDef alarma;

void RTC_CalendarConfig(void);
void RTC_CalendarShow(uint8_t *showtime, uint8_t *showdate);
//void RTC_CalendarShow(void);
void RTC_Init(void);
void Init_LSE (void);
extern void setAlarma (void);
void Timer_Callback_1s (void);



#endif 
