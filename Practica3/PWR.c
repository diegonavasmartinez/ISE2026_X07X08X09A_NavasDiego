#include "PWR.h"
//#include "SNTP.h"

RTC_HandleTypeDef RTCHandle;

static void SYSCLKConfig_STOP(void);

void PWR_Init(void){
	/* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Check and handle if the system was resumed from Standby mode */ 
  if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
  {
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);

    /* Exit Ethernet Phy from low power mode */
    ETH_PhyExitFromPowerDownMode();

    /* Infinite loop */
    while (1)
    {
      /* Toggle LED1 */
      //BSP_LED_Toggle(LED1);

      /* Insert a 100ms delay */
      HAL_Delay(100);
    }
  }

  /* Configure USER Button */
  init_PulsadorPWR();

  /* Wait until USER button is pressed to enter the Low Power mode */
//  while(BSP_PB_GetState(BUTTON_USER) == RESET)
//  {
//    /* Toggle LED1 */
//    //BSP_LED_Toggle(LED1);

//    /* Insert 1s Delay */
//    HAL_Delay(1000);
//  }
  /* Loop while USER Button is maintained pressed */
//  while(BSP_PB_GetState(BUTTON_USER) != RESET)
//  {
//  }

  /* Ethernet PHY must be in low power mode in order to have the lowest current consumption */
  //ETH_PhyEnterPowerDownMode();

//#if defined (SLEEP_MODE)
//  /* Sleep Mode Entry 
//      - System Running at PLL (100MHz)
//      - Flash 5 wait state
//      - Instruction and Data caches ON
//      - Prefetch ON
//      - Code running from Internal FLASH
//      - All peripherals disabled.
//      - Wake-up using EXTI Line (User Button)
//   */
//  SleepMode_Measure();
}

void SleepMode_Measure(void)
{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};

//  /* 1. Apagar Ethernet (Seguro si la pila de red no está enviando datos) */
//  __HAL_RCC_ETH_CLK_DISABLE();

//  /* 2. Configurar pines en Analógico para ahorrar energía */
//  // MUY IMPORTANTE: Excluir el pin del botón (PC13) para poder despertar
//  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  
//  GPIO_InitStruct.Pin = GPIO_PIN_All;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 
//  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//  
//  GPIO_InitStruct.Pin = GPIO_PIN_All & ~(GPIO_PIN_13); // <-- EXCEPCIÓN DEL BOTÓN
//  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//  
//  // (Haz lo mismo con los demás puertos D, E, F...)

//  /* 3. AHORA SÍ: APAGAR LOS RELOJES DE LOS BUSES GPIO */
//  __HAL_RCC_GPIOA_CLK_DISABLE();
//  __HAL_RCC_GPIOB_CLK_DISABLE();
//  // NO apagar GPIOC si el EXTI del botón necesita el reloj de ese puerto 
//  // (en STM32 normalmente EXTI funciona aunque el reloj del GPIO esté apagado, 
//  // pero es mejor dejarlo encendido si tienes dudas, consume muy poco).
//  
//  // Deshabilitar el Systick temporalmente
//  //HAL_SuspendTick();

//  /* 4. A DORMIR */
//  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

//  /* --- DESPERTAR --- */
//  
//  /* 5. Restaurar el Systick */
//  //HAL_ResumeTick();

//  /* 6. VOLVER A ENCENDER LOS RELOJES DE LOS BUSES */
//  __HAL_RCC_GPIOA_CLK_ENABLE();
//  __HAL_RCC_GPIOB_CLK_ENABLE();
//  __HAL_RCC_ETH_CLK_ENABLE();

//  /* 7. Reconfigurar los pines que necesites (LCD, LEDs, etc.) */
//  // Es importante volver a inicializar los pines que dejaste en modo analógico.
//  // Ej: LCD_init(); o volver a configurar los GPIO del Display.
//  
//  ETH_PhyExitFromPowerDownMode();
/* 1. Para evitar que la red atienda peticiones (Requisito PDF), 
        deshabilitamos la interrupción de Ethernet en el procesador.
        Así ignorará el tráfico web sin apagar el reloj (evitando el crash). */
//  HAL_NVIC_DisableIRQ(ETH_IRQn);
//	
//	__HAL_RCC_ETH_CLK_DISABLE();

//	ETH_PhyEnterPowerDownMode();
//	

//	
//  /* 2. Suspendemos el Tick del RTOS. 
//        Si no hacemos esto, el Systick despierta al micro cada 1 milisegundo. */
//  HAL_SuspendTick();
//	
//  /* 3. Entrar en modo SLEEP puro.
//        Solo se detiene la CPU, los periféricos mantienen su configuración. */
//  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

//  /* --- DESPERTAR (Por la interrupción EXTI13 del botón azul) --- */

//  /* 4. Restaurar el Tick del RTOS para que las tareas vuelvan a funcionar */
//  HAL_ResumeTick();
//	
//	 HAL_NVIC_EnableIRQ(ETH_IRQn);
//	__HAL_RCC_ETH_CLK_ENABLE();
//	ETH_PhyExitFromPowerDownMode();

//  /* 5. Volver a habilitar la interrupción de red para atender la web */
	/* 1. SILENCIAR ETHERNET A NIVEL HARDWARE */
  // Apagamos la interrupción MAC en el NVIC para que no nos despierte la red.
  HAL_NVIC_DisableIRQ(ETH_IRQn);
  
  // Apagamos el PHY para ahorrar energía extra (como pedía el ejemplo)
  ETH_PhyEnterPowerDownMode();

  /* 2. SILENCIAR EL SYSTICK DEL RTOS */
  // Si no hacemos esto, el reloj del RTOS (1ms) nos despierta.
  HAL_SuspendTick();

  /* 3. LIMPIAR BANDERAS RESIDUALES ANTES DE DORMIR */
  // Nos aseguramos de que no haya una interrupción del botón pendiente
  // que nos impida entrar al modo sleep.
  __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);

  /* ========================================================== */
  /* 4. ENTRAR EN MODO SLEEP                                    */
  /* Solo las interrupciones activas en el NVIC pueden despertar*/
  /* En este caso, solo queda vivo el EXTI15_10_IRQn (Botón)    */
  /* ========================================================== */
  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

  /* --- DESPERTAR (SOLO LLEGA AQUÍ SI SE PULSA EL BOTÓN AZUL) --- */

  /* 5. RESTAURAR EL SYSTICK PARA EL RTOS */
  HAL_ResumeTick();

  /* 6. RESTAURAR ETHERNET */
  // Despertamos el PHY primero
  ETH_PhyExitFromPowerDownMode();
  // Volvemos a habilitar las interrupciones MAC en el NVIC
  HAL_NVIC_EnableIRQ(ETH_IRQn);

}

void ETH_PhyEnterPowerDownMode(void)
{
  ETH_HandleTypeDef heth;
  GPIO_InitTypeDef GPIO_InitStruct;
  uint32_t phyregval = 0; 
   
  /* This part of code is used only when the ETH peripheral is disabled 
	   when the ETH is used in the application this initialization code 
	   is called in HAL_ETH_MspInit() function  ***********************/
	
	/* Enable GPIO clocks*/
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  
  /* Configure PA2: ETH_MDIO */
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL; 
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  /* Configure PC1: ETH_MDC */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
  /* Enable the ETH peripheral clocks */
  __HAL_RCC_ETH_CLK_ENABLE();
  
  /* Set ETH Handle parameters */
  heth.Instance = ETH;
  heth.Init.PhyAddress = LAN8742A_PHY_ADDRESS;

  /* Configure MDC clock: the MDC Clock Range configuration
	   depends on the system clock: 180Mhz/102 = 1.76MHz  */
  /* MDC: a periodic clock that provides the timing reference for 
	   the MDIO data transfer which shouldn't exceed the maximum frequency of 2.5 MHz.*/
  heth.Instance->MACMIIAR = (uint32_t)ETH_MACMIIAR_CR_Div102;

  /*****************************************************************/
  
  /* ETH PHY configuration in Power Down mode **********************/ 
  
  /* Read ETH PHY control register */
  //HAL_ETH_ReadPHYRegister(&heth, PHY_BCR, &phyregval);
  
  /* Set Power down mode bit */
  phyregval |= PHY_POWERDOWN;
  
  /* Write new value to ETH PHY control register */
  //HAL_ETH_WritePHYRegister(&heth, PHY_BCR, phyregval);
  
  /*****************************************************************/
  
  /* Disable periph CLKs */
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_ETH_CLK_DISABLE();
}


void ETH_PhyExitFromPowerDownMode(void)
{
   ETH_HandleTypeDef heth;
   GPIO_InitTypeDef GPIO_InitStruct;
   uint32_t phyregval = 0;
   
  /* ETH CLKs and GPIOs initialization ******************************/
  /* To be removed when the function is called from HAL_ETH_MspInit() when 
     exiting from Standby mode */
	
	/* Enable GPIO clocks*/
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  
  /* Configure PA2 */
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL; 
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  /* Configure PC1*/
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
  /* Enable ETH CLK */
  __HAL_RCC_ETH_CLK_ENABLE();
  /*****************************************************************/
  
  /* ETH PHY configuration to exit Power Down mode *****************/
  /* Set ETH Handle parameters */
  heth.Instance = ETH;
  heth.Init.PhyAddress = LAN8742A_PHY_ADDRESS;
  
  /* Configure MDC clock: the MDC Clock Range configuration
	   depends on the system clock: 180Mhz/102 = 1.76MHz  */
  /* MDC: a periodic clock that provides the timing reference for 
	   the MDIO data transfer which shouldn't exceed the maximum frequency of 2.5 MHz.*/
  heth.Instance->MACMIIAR = (uint32_t)ETH_MACMIIAR_CR_Div102; 
	
  /* Read ETH PHY control register */
  //HAL_ETH_ReadPHYRegister(&heth, PHY_BCR, &phyregval);
  
  /* check if the PHY  is already in power down mode */
  if ((phyregval & PHY_POWERDOWN) != RESET)
  {
    /* Disable Power down mode */
    phyregval &= ~ PHY_POWERDOWN;
    
    /* Write value to ETH PHY control register */
    //HAL_ETH_WritePHYRegister(&heth, PHY_BCR, phyregval);
  }
  /*****************************************************************/
}

/**
  * @brief  This function configures the system to enter Stop mode with RTC 
  *         clocked by LSE or LSI for current consumption measurement purpose.
  *         STOP Mode with RTC clocked by LSE/LSI
  *         =====================================   
  *           - RTC Clocked by LSE or LSI
  *           - Regulator in LP mode
  *           - HSI, HSE OFF and LSI OFF if not used as RTC Clock source
  *           - No IWDG
  *           - FLASH in deep power down mode
  *           - Automatic Wakeup using RTC clocked by LSE/LSI (~20s)
  * @param  None
  * @retval None
  */
void StopMode_Measure(void)
{
  //GPIO_InitTypeDef GPIO_InitStruct;  

  /* Disable USB Clock */
 // __HAL_RCC_USB_OTG_FS_CLK_DISABLE();

  /* Disable Ethernet Clock */
  __HAL_RCC_ETH_CLK_DISABLE();

  /* Configure all GPIO as analog to reduce current consumption on non used IOs */
  /* Enable GPIOs clock */
//  __HAL_RCC_GPIOA_CLK_ENABLE();
//  __HAL_RCC_GPIOB_CLK_ENABLE();
//  __HAL_RCC_GPIOC_CLK_ENABLE();
//  __HAL_RCC_GPIOD_CLK_ENABLE();
//  __HAL_RCC_GPIOE_CLK_ENABLE();
//  __HAL_RCC_GPIOF_CLK_ENABLE();
//  __HAL_RCC_GPIOG_CLK_ENABLE();
//  __HAL_RCC_GPIOH_CLK_ENABLE();
//  __HAL_RCC_GPIOI_CLK_ENABLE();
//  __HAL_RCC_GPIOJ_CLK_ENABLE();
//  __HAL_RCC_GPIOK_CLK_ENABLE();

//  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Pin = GPIO_PIN_All;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 
//  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
//  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
//  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
//  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
//  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct); 
//  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
//  HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct); 
//  HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);

//  /* Disable GPIOs clock */
//  __HAL_RCC_GPIOA_CLK_DISABLE();
//  __HAL_RCC_GPIOB_CLK_DISABLE();
//  __HAL_RCC_GPIOC_CLK_DISABLE();
//  __HAL_RCC_GPIOD_CLK_DISABLE();
//  __HAL_RCC_GPIOE_CLK_DISABLE();
//  __HAL_RCC_GPIOF_CLK_DISABLE();
//  __HAL_RCC_GPIOG_CLK_DISABLE();
//  __HAL_RCC_GPIOH_CLK_DISABLE();
//  __HAL_RCC_GPIOI_CLK_DISABLE();
//  __HAL_RCC_GPIOJ_CLK_DISABLE();
//  __HAL_RCC_GPIOK_CLK_DISABLE();

  /* Configure RTC prescaler and RTC data registers as follows:
  - Hour Format = Format 24
  - Asynch Prediv = Value according to source clock
  - Synch Prediv = Value according to source clock
  - OutPut = Output Disable
  - OutPutPolarity = High Polarity
  - OutPutType = Open Drain */ 
//  RTCHandle.Instance = RTC;
//  RTCHandle.Init.HourFormat = RTC_HOURFORMAT_24;
//  RTCHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
//  RTCHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
//  RTCHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
//  RTCHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
//  RTCHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

//  if(HAL_RTC_Init(&RTCHandle) != HAL_OK)
//  {
//    /* Initialization Error */
//    //Error_Handler(); 
//		while(1){
//			osDelay(100);
//		}
//  }

  /*## Configure the Wake up timer ###########################################*/
  /*  RTC Wakeup Interrupt Generation:
      Wakeup Time Base = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSI))
      Wakeup Time = Wakeup Time Base * WakeUpCounter 
                  = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSI)) * WakeUpCounter
      ==> WakeUpCounter = Wakeup Time / Wakeup Time Base

      To configure the wake up timer to 20s the WakeUpCounter is set to 0xA017:
        RTC_WAKEUPCLOCK_RTCCLK_DIV = RTCCLK_Div16 = 16 
        Wakeup Time Base = 16 /(~32.768KHz) = ~0,488 ms
        Wakeup Time = ~20s = 0,488ms  * WakeUpCounter
        ==> WakeUpCounter = ~20s/0,488ms = 40983 = 0xA017 */

  /* Disable Wake-up timer */
//  HAL_RTCEx_DeactivateWakeUpTimer(&RTCHandle);

//  /* Enable Wake-up timer */
//  HAL_RTCEx_SetWakeUpTimer_IT(&RTCHandle, 0xA017, RTC_WAKEUPCLOCK_RTCCLK_DIV16);

//  /* FLASH Deep Power Down Mode enabled */
//  HAL_PWREx_EnableFlashPowerDown();

  /*## Enter Stop Mode #######################################################*/
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

  /* Configures system clock after wake-up from STOP: enable HSI, PLL and select 
     PLL as system clock source (HSI and PLL are disabled in STOP mode) */
  SYSCLKConfig_STOP();
	
	HAL_ResumeTick();

  /* Exit Ethernet Phy from low power mode */
  ETH_PhyExitFromPowerDownMode();

  /* Disable Wake-up timer */
//  if(HAL_RTCEx_DeactivateWakeUpTimer(&RTCHandle) != HAL_OK)
//  {
//    /* Initialization Error */
//    //Error_Handler(); 
//		osDelay(100);
//  }
}

static void SYSCLKConfig_STOP(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  uint32_t pFLatency = 0;
  
  /* Get the Oscillators configuration according to the internal RCC registers */
  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);
  
  /* After wake-up from STOP reconfigure the system clock: Enable HSE and PLL */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    //Error_Handler();
		osDelay(100);
  }

  /* Get the Clocks configuration according to the internal RCC registers */
  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &pFLatency);
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, pFLatency) != HAL_OK)
  {
    //Error_Handler();
		osDelay(100);
  }
}

void init_PulsadorPWR (void){

  GPIO_InitTypeDef GPIO_InitStruct;

  /*Enable clock to GPIO-C*/
  __HAL_RCC_GPIOC_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  
}

