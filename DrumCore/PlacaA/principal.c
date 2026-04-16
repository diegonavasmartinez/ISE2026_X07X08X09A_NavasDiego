	#include "principal.h"                          // CMSIS RTOS header file
	 
	/*----------------------------------------------------------------------------
	 *      Thread 1 'Thread_Name': Sample thread
	 *---------------------------------------------------------------------------*/
	 
	osThreadId_t tid_principal;                        // thread id

	/*
	typedef enum{
		HORAS_1,HORAS_2,MINUTOS_1,MINUTOS_2,SEGUNDOS_1,SEGUNDOS_2,NUM_DIG
	}SelDigito;

	SelDigito seleccion = HORAS_1;
	*/
	typedef enum{
		HORAS, //0
		MINUTOS,  //1
		SEGUNDOS,  //2
	}Clock_t;

	Clock_t select = HORAS;


//	MSGQUEUELCD_OBJ_t infoLCD;

	#define NUM_CARPETAS 3

	const uint8_t archivos[NUM_CARPETAS]={2,3,1};//numero de canciones por carpeta
	//const uint8_t archivos[NUM_CARPETAS]={1,2,0};//numero de canciones por carpeta
	static uint8_t cancionActual =1;
	static uint8_t carpetaActual=1;

	bool play = false;
	bool SD = true;

	bool loop=false;

	const uint8_t trama_reply[10] = {0x7E,0xFF,0x06,0x48,0x01,0x00,0x00,0xFE,0xB2,0xEF};

	//FUNCIONES ORGANIZADAS
	void inicializarMP3(void);

	void principal (void *argument);                   // thread function
	 
	int Init_principal (void) {
		
		tid_principal = osThreadNew(principal, NULL, NULL);
		if (tid_principal == NULL) {
			return(-1);
		}
	 
		return(0);
	}
	 


	void principal (void *argument) {	
		
		osStatus_t estadoJoy;
		uint8_t Send_buf[TAM_TRAMA_MP3];
		float temperatura;


		inicializarMP3();
		
		while (1) {
					 
		}
	}

	void inicializarMP3(void){
		
		uint8_t Send_buf[TAM_TRAMA_MP3];
		
		sendCommand(CMD_RESET, 0x00, 0x00, Send_buf);
		osMessageQueuePut(getQueueIdMP3Send(),Send_buf, 0U, 0U);
		//envioPC(Send_buf);
		
		sendCommand(CMD_SEL_DEV, 0x00, 0x02, Send_buf);
		osMessageQueuePut(getQueueIdMP3Send(),&Send_buf, 0U, 0U);
		
		sendCommand(CMD_SET_VOLUME, 0x00, 0x50, Send_buf);
		osMessageQueuePut(getQueueIdMP3Send(),&Send_buf, 0U, 0U);
		
		sendCommand(CMD_PLAY_FOLDER_FILE, 0x01, 0x01, Send_buf);
		osMessageQueuePut(getQueueIdMP3Send(),Send_buf, 0U, 0U);
		
		osDelay(100);
		//envioPC(Send_buf);
	}

//	void comprobacionTarjeta(uint8_t *Send_buf){
//		
//			MSGQUEUE_OBJ_t rgb;
//			MSGQUEUEPWM_OBJ_t pwm;
//			osStatus_t estado;
//			uint8_t Send_buf_Rx[TAM_TRAMA_MP3];

//		
//			estado = osMessageQueueGet(getQueueIdMP3Receive(),&Send_buf_Rx,NULL,0U);
//					
//				if(estado== osOK && Send_buf_Rx[3] == 0x3B){
//						SD=false;
//						play = false;
//						loop=false;
//						osTimerStop(timerTiempoCancion);
//								
//						modActual = REPOSO;
//						rgb.color=RED;
//						osMessageQueuePut(getIdRGBQueue(),&rgb, 0U, 0U);
//								
//						pwm.periodo=40;
//						pwm.duracion=2000;
//						osMessageQueuePut(getQueueIdPWM(),&pwm,NULL, osWaitForever);
//								
//						sendCommand(CMD_SLEEP_MODE, 0x00, 0x00, Send_buf);
//						osMessageQueuePut(getQueueIdMP3Send(),Send_buf, 0U, 0U);
//						envioPC(Send_buf);	
//					
//				}else if(estado== osOK && Send_buf_Rx[3] == 0x3A){
//						SD=true;
//						play = false;
//								
//						rgb.color=GREEN;
//						osMessageQueuePut(getIdRGBQueue(),&rgb, 0U, 0U);
//								
//						pwm.periodo=100;
//						pwm.duracion=500;
//						osMessageQueuePut(getQueueIdPWM(),&pwm,NULL, osWaitForever);
//						envioPC(Send_buf);
//					
//				}
//				if (estado== osOK && Send_buf_Rx[3] == 0x3D) {   // Fin de cancion

//						if(loop==false){
//								play = false;
//								osTimerStop(timerTiempoCancion);

//								// Opcional: resetear contador
//								segCancion = 0;
//								minCancion = 0;

//								// Opcional: LED
//								rgb.color = YELLOW;
//								osMessageQueuePut(getIdRGBQueue(), &rgb, 0U, 0U);
//						}else{
//								play = true;
//							
//								segCancion = 0;
//								minCancion = 0;
//							
//							
//								if(carpetaActual==NUM_CARPETAS && cancionActual == archivos[carpetaActual-1]){
//									carpetaActual=1;
//									cancionActual=1;
//								}else if(carpetaActual!=NUM_CARPETAS && cancionActual == archivos[carpetaActual-1]){
//									carpetaActual=carpetaActual+1;
//									cancionActual=1;
//								}else  if(carpetaActual!=NUM_CARPETAS && cancionActual != archivos[carpetaActual-1]){
//									cancionActual=cancionActual+1;
//								}
//								
//								sendCommand(CMD_PLAY_FOLDER_FILE, carpetaActual, cancionActual, Send_buf);
//								osMessageQueuePut(getQueueIdMP3Send(),Send_buf, 0U, 0U);
//								envioPC(Send_buf);
//								inicioTimerCancion();						

//								// Opcional: LED
//								
//							}
//	}
//}


