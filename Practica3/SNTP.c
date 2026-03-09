#include "SNTP.h"

netStatus SNTP_OK;

struct tm t_SNTP;


const NET_ADDR4 ntp_server = { NET_ADDR_IP4, 0, 216, 239, 35, 0 };
static void time_callback (uint32_t seconds, uint32_t seconds_fraction);
 
void get_time (void) {
  if (netSNTPc_GetTime ((NET_ADDR *)&ntp_server, time_callback) == netOK) {
    //printf ("SNTP request enviada.\n");
//		while(1){
//			osDelay(1000);
//		}
  }
  else {
    //printf ("SNTP no esta preparado o los parametros no son adecuados.\n");
		while(1){
			osDelay(1000);
		}
  }
}
 
static void time_callback (uint32_t seconds, uint32_t seconds_fraction) {
  if (seconds == 0) {
//    printf ("Server not responding or bad response.\n");
		while(1){
			osDelay(1000);
		}
  }
  else {
		t_SNTP = *localtime(&seconds); 
		sdatestructureget.Year = t_SNTP.tm_year - 100; ///PASAMOS A FROMATO RTC
    sdatestructureget.Month = t_SNTP.tm_mon + 1; //FORMATO RTC
    sdatestructureget.Date = t_SNTP.tm_mday;
    sdatestructureget.WeekDay = t_SNTP.tm_wday;
		
		if(HAL_RTC_SetDate(&RtcHandle, &sdatestructureget, RTC_FORMAT_BIN) != HAL_OK)
    {
      /* Initialization Error */
     // Error_Handler();
			printf("Se ha producido un error al intentar establecer la fecha actual");
    }
		
		stimestructureget.Hours = (t_SNTP.tm_hour > 23) ? 0 : (t_SNTP.tm_hour + 1);
    stimestructureget.Minutes = t_SNTP.tm_min;
    stimestructureget.Seconds = t_SNTP.tm_sec;
    stimestructureget.TimeFormat = (t_SNTP.tm_hour < 12) ? RTC_HOURFORMAT12_AM : RTC_HOURFORMAT12_PM;
    stimestructureget.DayLightSaving = t_SNTP.tm_isdst;
    stimestructureget.StoreOperation = RTC_STOREOPERATION_RESET;
    
    if (HAL_RTC_SetTime(&RtcHandle, &stimestructureget	, RTC_FORMAT_BIN) != HAL_OK)
    {
    /* Initialization Error */
     printf("Se ha producido un error al intentar poner el reloj en hora.");
    }
    
    HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR1, 0x32F2);
  }
}

void init_Pulsador (void){

  GPIO_InitTypeDef GPIO_InitStruct;

  /*Enable clock to GPIO-C*/
  __HAL_RCC_GPIOC_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  
}
