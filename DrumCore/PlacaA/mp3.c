#include "mp3.h"
 
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_MP3_tx;                        // thread id
osMessageQueueId_t idMsgMP3Send;

osThreadId_t tid_MP3_rx;                        // thread id
osMessageQueueId_t idMsgMP3Receive;

static ARM_DRIVER_USART * USARTdrv=&Driver_USART2;
 
                 // thread function
void MP3_tx (void *argument);
void MP3_rx (void *argument);
void MP3_callback(uint32_t event);

//Enum para automata
typedef enum{
	InitState,
	DefaultState
}EstadoMP3_3;

EstadoMP3_3 estadoRxMP3=InitState;

 
int Init_MP3 (void) {
 
  tid_MP3_tx = osThreadNew(MP3_tx, NULL, NULL);
	tid_MP3_rx = osThreadNew(MP3_rx, NULL, NULL);
	idMsgMP3Send=osMessageQueueNew(10,sizeof(uint8_t[TAM_TRAMA_MP3]),NULL);
	idMsgMP3Receive=osMessageQueueNew(10,sizeof(uint8_t[TAM_TRAMA_MP3]),NULL);
  if (tid_MP3_tx == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void MP3_tx (void *argument) {
	
	static uint8_t sendMessage[TAM_TRAMA_MP3];
	static uint8_t Send_buf[TAM_TRAMA_MP3];
	
	 /*Initialize the USART driver */
    USARTdrv->Initialize(MP3_callback);//Lo inicializo para la llamada a callback
    /*Power up the USART peripheral */
    USARTdrv->PowerControl(ARM_POWER_FULL);
    /*Configure the USART to 4800 Bits/sec */
    USARTdrv->Control(ARM_USART_MODE_ASYNCHRONOUS |//Es una comunicacion asincrona bit a bit
                      ARM_USART_DATA_BITS_8 |
                      ARM_USART_PARITY_NONE |
                      ARM_USART_STOP_BITS_1 |
                      ARM_USART_FLOW_CONTROL_NONE, 9600);
     
    /* Enable Receiver and Transmitter lines */
    USARTdrv->Control (ARM_USART_CONTROL_TX, 1);//Habilitar la transmision
		USARTdrv->Control (ARM_USART_CONTROL_RX, 1);//Habilitar la transmision
	
//		sendCommand(CMD_RESET, 0x00, 0x00, Send_buf);
//		osMessageQueuePut(idMsgMP3Send,&Send_buf, 0U, 0U);
//	
//		sendCommand(CMD_SEL_DEV, 0x00, 0x02, Send_buf);
//		osMessageQueuePut(idMsgMP3Send,&Send_buf, 0U, 0U);
//		
//		sendCommand(CMD_SET_VOLUME, 0x00, 0x50, Send_buf);
//		osMessageQueuePut(idMsgMP3Send,&Send_buf, 0U, 0U);
		
//		sendCommand(CMD_PLAY_FOLDER_FILE, 0x01, 0x01, Send_buf);
//		osMessageQueuePut(getQueueIdMP3Send(),Send_buf, 0U, 0U);
	
	while(1){
		osMessageQueueGet(idMsgMP3Send, &sendMessage, NULL, osWaitForever);
		
		USARTdrv->Send(&sendMessage, sizeof(sendMessage));          //La USART escucha, se pone operativo, pero no es bloqueante
    osThreadFlagsWait(ARM_USART_EVENT_SEND_COMPLETE,osFlagsWaitAny, osWaitForever);
		osDelay(50);
		
	}
}

void MP3_rx (void *argument){
	static char byte;
	static char Send_buf_rx[TAM_TRAMA_MAX];
	static int i=0;
	static uint32_t status;
	
	while(1){
		USARTdrv->Receive(&byte,1);
		status=osThreadFlagsWait(ARM_USART_EVENT_RECEIVE_COMPLETE, osFlagsWaitAny, osWaitForever);
		
			if(status==ARM_USART_EVENT_RECEIVE_COMPLETE){
				if(estadoRxMP3==InitState){
					if(byte==0x7E){
						i=0;
						estadoRxMP3=DefaultState;
						Send_buf_rx[i]=byte;
						i++;
					}
				}else{
					if(byte!=0xEF){
						Send_buf_rx[i]=byte;
						i++;
					}else{
						estadoRxMP3=InitState;
						Send_buf_rx[i]=byte;
						osMessageQueuePut(idMsgMP3Receive, &Send_buf_rx, 0U, 0U);
					}
				}
			}else{
				osThreadYield();
			}
		}
}

void MP3_callback(uint32_t event)
{ 
	if(event & ARM_USART_EVENT_SEND_COMPLETE){
		osThreadFlagsSet(tid_MP3_tx, ARM_USART_EVENT_SEND_COMPLETE);
	}
	if(event & ARM_USART_EVENT_RECEIVE_COMPLETE){
		osThreadFlagsSet(tid_MP3_rx, ARM_USART_EVENT_RECEIVE_COMPLETE);
	}

}

osMessageQueueId_t getQueueIdMP3Send(void){
	return idMsgMP3Send;
}

osMessageQueueId_t getQueueIdMP3Receive(void){
	return idMsgMP3Receive;
}

void sendCommand(uint8_t command, uint8_t dat1, uint8_t dat2, uint8_t *Send_buf){
	
	Send_buf[0]=0x7E;
	Send_buf[1]=0xFF;
	Send_buf[2]=0x06;
	Send_buf[3]=command;
	Send_buf[4]=0x00;//Feedback
	Send_buf[5]=dat1;
	Send_buf[6]=dat2;
	Send_buf[7]=0xEF;
	
}

