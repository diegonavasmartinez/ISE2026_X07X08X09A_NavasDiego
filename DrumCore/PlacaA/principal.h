#ifndef __PRINCIPAL_H
#define __PRINCIPAL_H

//Librerias hilos
#include "cmsis_os2.h"
#include "mp3.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_hal.h"
 
extern osThreadId_t tid_principal;                        // thread id

int Init_principal (void);
void principal (void *argument);



#endif
