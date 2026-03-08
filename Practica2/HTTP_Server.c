/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HTTP_Server.c
 * Purpose: HTTP Server example
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include "main.h"
#include "rl_net.h"                     // Keil.MDK-Pro::Network:CORE
#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "Board_LED.h"                  // ::Board Support:LED
#include "Board_Buttons.h"              // ::Board Support:Buttons
//#include "Board_ADC.h"                  // ::Board Support:A/D Converter
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
//#include "GLCD_Config.h"                // Keil.MCBSTM32F400::Board Support:Graphic LCD
#include "LCD.h"
#include "adc.h"
#include  "RTC.h"
#include <stdio.h>

// Main stack size must be multiple of 8 Bytes
#define APP_MAIN_STK_SZ (1024U)
uint64_t app_main_stk[APP_MAIN_STK_SZ / 8];
const osThreadAttr_t app_main_attr = {
  .stack_mem  = &app_main_stk[0],
  .stack_size = sizeof(app_main_stk)
};

//extern GLCD_FONT GLCD_Font_6x8;
//extern GLCD_FONT GLCD_Font_16x24;

extern uint16_t AD_in          (uint32_t ch);
extern uint8_t  get_button     (void);
extern void     netDHCP_Notify (uint32_t if_num, uint8_t option, const uint8_t *val, uint32_t len);

extern bool LEDrun;
extern char lcd_text[2][20+1];

uint8_t aShowTime[10] = {0};
uint8_t aShowDate[10] = {0}; 

//extern osThreadId_t TID_Display;
//extern osThreadId_t TID_Led;
//extern osThreadId_t TID_RTC;


bool LEDrun;
char lcd_text[2][20+1] = { "LCD line 1",
                           "LCD line 2" };

/* Thread IDs */
osThreadId_t TID_Display;
osThreadId_t TID_Led;
osThreadId_t TID_RTC;
osThreadId_t TID_Alarma;
osThreadId_t TID_SNTP;
													 
osTimerId_t id_tim_1s;
osTimerId_t id_tim_6s;
osTimerId_t id_tim_3m;
uint16_t timer_1seg=0;
													 
//CALLBACKS TIMERS
static void SetTimers (void);
void Timer_Callback_1s (void);

/* Thread declarations */
static void BlinkLed (void *arg);
static void Display  (void *arg);
static void parpadeoAlarma (void *arg);
static void actualizarRTC (void *arg);
static void sincroSNTP (void *arg);
													 
__NO_RETURN void app_main (void *arg);


static void SetTimers (void){ 
	id_tim_1s= osTimerNew((osTimerFunc_t)Timer_Callback_1s, osTimerPeriodic, NULL,NULL);
  id_tim_6s= osTimerNew((osTimerFunc_t)Timer_Callback_1s, osTimerPeriodic, NULL,NULL);
	id_tim_3m= osTimerNew((osTimerFunc_t)Timer_Callback_1s, osTimerPeriodic, NULL,NULL);

}

void Timer_Callback_1s (void){
 
 if(timer_1seg<10){
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
	   timer_1seg++;
 }else{
   osTimerStop(id_tim_1s);
	 timer_1seg=0;
 }
}

void Timer_Callback_6s (void){
	osTimerStart(id_tim_1s, 200);
	SNTP_INIT(); //reinicio por primera vez del SNTP como dice el enunciado (re-configuracion)
	osTimerStart(id_tim_3m,18000); //18000ms
}
void Timer_Callback_3m (void){
	osTimerStart(id_tim_1s, 200);
	SNTP_INIT(); //re sincronizacion a los 3 minutos
	
}

uint16_t AD_in (uint32_t ch) {
  // En lugar de ADC_StartConversion...
  // Usamos tu función de adc.c (asumiendo que quieres el valor crudo)
  static float raw=0;
	static uint16_t value;
	ADC_HandleTypeDef adchandle;

	
	if(ch==0){
		ADC_Init_Single_Conversion(&adchandle, ADC1);
	}
	
	raw=ADC_getVoltage(&adchandle, 10);
	value=(uint16_t)raw;
	return (uint16_t)(raw*1250);
}

/* Read digital inputs */
uint8_t get_button (void) {
  return ((uint8_t)Buttons_GetState ());
}

/* IP address change notification */
void netDHCP_Notify (uint32_t if_num, uint8_t option, const uint8_t *val, uint32_t len) {

  (void)if_num;
  (void)val;
  (void)len;

  if (option == NET_DHCP_OPTION_IP_ADDRESS) {
    /* IP address change, trigger LCD update */
    osThreadFlagsSet (TID_Display, 0x01);
  }
}


static __NO_RETURN void sincroSNTP (void *arg) {
	
(void)arg;
	uint8_t flagPulsador= 0x0U;
	
  while(1) {
		flagPulsador= osThreadFlagsWait(0x01U, osFlagsWaitAll,osWaitForever);
		
		if(flagPulsador==0x01){		
			stimestructureget.Hours=0x00;
			stimestructureget.Minutes=0x00;
			stimestructureget.Seconds=0x00;
			
			sdatestructureget.Date=0x01;
			sdatestructureget.Month=0x01;
			sdatestructureget.Year=0x01;
			sdatestructureget.WeekDay=0x00;
			for(int i= 0; i < 10; i++){
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
        osDelay(100);
      }
		}
  }
}

/*----------------------------------------------------------------------------
  Thread 'Display': LCD display handler
 *---------------------------------------------------------------------------*/
static __NO_RETURN void Display (void *arg) {

	MSGQUEUELCD_OBJ_t msg;
  LCD_reset();
	LCD_init();
	clean_buffer();

  while(1) {

    osThreadFlagsWait (0x50U, osFlagsWaitAny, osWaitForever);
		
		msg.reset = true;
    msg.linea = 1;
		snprintf(msg.cadena, sizeof(msg.cadena), "%s", lcd_text[0]);
    write_LCD(msg.cadena, msg.linea, msg.reset);

    msg.linea = 2;
    msg.reset = false; // No reset para no borrar la línea 1
    snprintf(msg.cadena, sizeof(msg.cadena), "%s", lcd_text[1]);
		write_LCD(msg.cadena, msg.linea, msg.reset);
  }
}

/*----------------------------------------------------------------------------
  Thread 'BlinkLed': Blink the LEDs on an eval board
 *---------------------------------------------------------------------------*/
static __NO_RETURN void BlinkLed (void *arg) {
  const uint8_t led_val[16] = { 0x48,0x88,0x84,0x44,0x42,0x22,0x21,0x11,
                                0x12,0x0A,0x0C,0x14,0x18,0x28,0x30,0x50 };
  uint32_t cnt = 0U;

  (void)arg;

  LEDrun = true;
  while(1) {
    /* Every 100 ms */
    if (LEDrun == true) {
      LED_SetOut (led_val[cnt]);
      if (++cnt >= sizeof(led_val)) {
        cnt = 0U;
      }
    }
    osDelay (100);
  }
}



static __NO_RETURN void parpadeoAlarma (void *arg) {
	
	uint8_t FlagAlarma= 0x0U;
//SI NO PONES LA INTERRUPCION DE RTC JAMAS SALTA EL LED
  while(1) {
		FlagAlarma= osThreadFlagsWait(0x01U, osFlagsWaitAll,osWaitForever);
		
		if(FlagAlarma== 0x01){
			stimestructureget.Hours=0x00;
			stimestructureget.Minutes=0x00;
			stimestructureget.Seconds=0x00;
			
			sdatestructureget.Date=0x01;
			sdatestructureget.Month=0x01;
			sdatestructureget.Year=0x01;
			sdatestructureget.WeekDay=0x00;
			
			for(int i=0; i<10; i++){
			    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
          osDelay(500);
			}
		
		}
		
  }
}

static __NO_RETURN void actualizarRTC (void *arg) {
	
//	osTimerStart(id_tim_6s, 6000);

  while(1) {
		RTC_CalendarShow(aShowTime, aShowDate);
		
		
		memcpy(lcd_text[0], aShowTime, sizeof(aShowTime));
    lcd_text[0][sizeof(aShowTime)] = '\0';
    
    memcpy(lcd_text[1], aShowDate, sizeof(aShowDate));
    lcd_text[1][sizeof(aShowDate)] = '\0';
    
    //Envía al hilo del LCD para escribir
    osThreadFlagsSet (TID_Display, 0x50); 
		
		osDelay(1000);
		
  }
}

/*----------------------------------------------------------------------------
  Main Thread 'main': Run Network
 *---------------------------------------------------------------------------*/
__NO_RETURN void app_main (void *arg) {
  (void)arg;

  LED_Initialize();
  Buttons_Initialize();
	ADC1_pins_F429ZI_config();
	
  netInitialize ();
	
	RTC_Init();
	setAlarma();
	
	SetTimers();

  //TID_Led     = osThreadNew (BlinkLed, NULL, NULL);
  TID_Display = osThreadNew (Display,  NULL, NULL);
	TID_RTC			 = osThreadNew (actualizarRTC,  NULL, NULL);
	TID_Alarma	 = osThreadNew (parpadeoAlarma,  NULL, NULL);
	TID_SNTP = osThreadNew (sincroSNTP,  NULL, NULL);

  osThreadExit();
}
