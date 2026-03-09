#include "RTC.h"
#include "LCD.h"

RTC_DateTypeDef sdatestructureget;
RTC_TimeTypeDef stimestructureget;
RTC_HandleTypeDef RtcHandle;
RTC_AlarmTypeDef alarma;

RTC_DateTypeDef sdatestructure;
RTC_TimeTypeDef stimestructure;


void Init_LSE (void)
{
  RCC_OscInitTypeDef        RCC_OscInitStruct={0};
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct={0};

  /*##-2- Configure LSE as RTC clock source ###################################*/
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSE;
  //RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	
	
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  { 
   while(1);
  }
  
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  { 
    while(1);
  }
  
  /*##-3- Enable RTC peripheral Clocks #######################################*/
  /* Enable RTC Clock */
  __HAL_RCC_RTC_ENABLE();
}

/*##-1- Configure the RTC peripheral #######################################*/
  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follows:
      - Hour Format    = Format 24
      - Asynch Prediv  = Value according to source clock
      - Synch Prediv   = Value according to source clock
      - OutPut         = Output Disable
      - OutPutPolarity = High Polarity
      - OutPutType     = Open Drain */ 
void RTC_Init(void){
	
	Init_LSE();
	
  RtcHandle.Instance = RTC; 
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  __HAL_RTC_RESET_HANDLE_STATE(&RtcHandle);
	
	//Inicializacion del  RTC
	
	
	if (HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    /* Initialization Error */
    //Error_Handler();
		while(1);
  }
  
  /* Turn on LED1 */
  //BSP_LED_On(LED1);

  /*##-2- Check if Data stored in BackUp register1: No Need to reconfigure RTC#*/
  /* Read the Back Up Register 1 Data */
  if (HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR1) != 0x32F2)
	{
//    /* Configure RTC Calendar */
		setAlarma();
    RTC_CalendarConfig();
  }
  else
  {
		//RTC_CalendarConfig();
    /* Check if the Power On Reset flag is set */
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET)
    {
      /* Turn on LED2: Power on reset occurred */
     // BSP_LED_On(LED2);
    }
    /* Check if Pin Reset flag is set */
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET)
    {
      /* Turn on LED1: External reset occurred */
     // BSP_LED_On(LED1);
    }
    /* Clear source Reset Flag */
    __HAL_RCC_CLEAR_RESET_FLAGS();
  }

}	
	
void RTC_CalendarConfig(void)
{
  /*##-1- Configure the Date #################################################*/
  /* Set Date: Tuesday February 18th 2014 */
  sdatestructure.Year = 0x26;
  sdatestructure.Month = RTC_MONTH_MARCH;
  sdatestructure.Date = 0x06;
  sdatestructure.WeekDay = RTC_WEEKDAY_FRIDAY;
  
  if(HAL_RTC_SetDate(&RtcHandle,&sdatestructure,RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    //Error_Handler();
		while(true);
  }

  /*##-2- Configure the Time #################################################*/
  /* Set Time: 02:00:00 */
  stimestructure.Hours = 0x11;
  stimestructure.Minutes = 0x28;
  stimestructure.Seconds = 0x00;
  stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;

  if (HAL_RTC_SetTime(&RtcHandle, &stimestructure, RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
//    Error_Handler();
		while(true);
  }

  /*##-3- Writes a data in a RTC Backup data Register1 #######################*/
  HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR1, 0x32F2);
}

/**
  * @brief  Display the current time and date.
  * @param  showtime : pointer to buffer
  * @param  showdate : pointer to buffer
  * @retval None
  */
//void RTC_CalendarShow(uint8_t *showtime, uint8_t *showdate)
//{
void RTC_CalendarShow(uint8_t *showtime, uint8_t *showdate){

	/* Get the RTC current Time */
  HAL_RTC_GetTime(&RtcHandle, &stimestructureget, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, RTC_FORMAT_BIN);
  /* Display time Format : hh:mm:ss */
  sprintf((char *)showtime, "%02d:%02d:%02d", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
  /* Display date Format : mm-dd-yy */
  sprintf((char *)showdate, "%02d-%02d-%04d", sdatestructureget.Month, sdatestructureget.Date, 2000 + sdatestructureget.Year);
	
}

void setAlarma (void){
	HAL_RTC_GetTime(&RtcHandle, &stimestructureget, RTC_FORMAT_BIN); //obtengo la hora
	alarma.AlarmTime.Hours= stimestructureget.Hours;// RTC_Time.Hours;
	alarma.AlarmTime.Minutes=stimestructureget.Minutes;//RTC_Time.Minutes;
	alarma.AlarmTime.Seconds=0;
	
	alarma.AlarmTime.DayLightSaving=RTC_DAYLIGHTSAVING_NONE;
	alarma.AlarmTime.StoreOperation=RTC_STOREOPERATION_RESET;
	alarma.AlarmMask= RTC_ALARMMASK_DATEWEEKDAY | RTC_ALARMMASK_HOURS | RTC_ALARMMASK_MINUTES; //en mascara pongo lo que no quiero tener en cuenta ('enmascarar')
	alarma.AlarmDateWeekDaySel=RTC_ALARMDATEWEEKDAYSEL_DATE; 
	alarma.AlarmDateWeekDay=sdatestructureget.WeekDay;  //monday
	alarma.Alarm=RTC_ALARM_A;
	
	HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn); //Habilita ambas alarmas
	
  __HAL_RTC_ALARM_EXTI_ENABLE_IT();       // Habilita la interrupción
	__HAL_RTC_ALARM_EXTI_ENABLE_RISING_EDGE();  // Sensibilidad a flanco de subi
    // Configura la alarma en el RTC
    if (HAL_RTC_SetAlarm_IT(&RtcHandle, &alarma, RTC_FORMAT_BIN) != HAL_OK) {
       while (1);
    }

}




	