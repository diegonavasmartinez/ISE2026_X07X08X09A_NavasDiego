#ifndef __LCD_H
#define __LCD_H


#include "stm32f4xx_hal.h" 
#include "cmsis_os2.h"  
#include "Driver_SPI.h"




extern unsigned char buffer[512];
//extern GPIO_InitTypeDef GPIO_InitStruct;

extern osThreadId_t tid_LCD;
extern osMessageQueueId_t id_MsgQueueLCD;

typedef struct{
	char cadena[21];
	int linea;
	bool reset;
}MSGQUEUELCD_OBJ_t;

//funciones
int Init_LCD (void) ;
void LCD_reset(void);
void LCD_wr_data(unsigned char data);
void LCD_wr_cmd(unsigned char cmd);
void LCD_update(void);
void LCD_symbolToLocalBuffer_L1(uint8_t symbol);
void LCD_symbolToLocalBuffer_L2(uint8_t symbol);
void LCD_update_L1(void);
void LCD_update_L2(void);
void symbolToLocalBuffer(uint8_t line,uint8_t symbol, bool reset);
void write_LCD(char cadena[], int linea, bool reset);
void clean_buffer(void);
void LCD_init(void);



	
#endif 

