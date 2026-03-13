#ifndef __PWR_H
#define __PWR_H

#include "stm32f4xx_hal.h"      // Ajusta según tu familia, p.ej., stm32f4xx_hal.h
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_pwr.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_eth.h"    // ETH peripheral
#include "stm32f4xx_hal_rtc.h"

// Dirección del PHY LAN8742A (por defecto 0)
#define LAN8742A_PHY_ADDRESS  0

// Registro de control básico del PHY
//#define PHY_BCR               0x00

//// Bit de Power Down en el BCR
//#define PHY_POWERDOWN         0x0800

/* Valores para un cristal de reloj (LSE) de 32.768 kHz */
#define RTC_ASYNCH_PREDIV  127
#define RTC_SYNCH_PREDIV   255

#define SLEEP_MODE
/* #define STOP_MODE */
/* #define STOP_UNDERDRIVE_MODE */
/* #define STANDBY_MODE */
/* #define STANDBY_RTC_MODE */
/* #define STANDBY_RTC_BKPSRAM_MODE */

void PWR_Init(void);
void init_PulsadorPWR (void);
void SleepMode_Measure(void);
void StopMode_Measure(void);
void StopUnderDriveMode_Measure(void);
void StandbyMode_Measure(void);
void StandbyRTCMode_Measure(void);
void StandbyRTCBKPSRAMMode_Measure(void);
void ETH_PhyEnterPowerDownMode(void);
void ETH_PhyExitFromPowerDownMode(void);

#endif
