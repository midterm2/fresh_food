
#ifndef UART_H
#define	UART_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "defines.h"

#define UART_RX_BUFFER_SIZE             512
#define UART_TX_BUFFER_SIZE             512


#define UART_LOW_SPEED_MODE         1
#if (UART_LOW_SPEED_MODE==0)
    #define BAUDRATE_DIV            4
#else
    #define BAUDRATE_DIV            16
#endif

#define BRGVAL_9600             ((FCY/9600)/BAUDRATE_DIV)-1         /* 259 */
#define BRGVAL_19200            ((FCY/19200)/BAUDRATE_DIV)-1
#define BRGVAL_38400            ((FCY/38400)/BAUDRATE_DIV)-1
#define BRGVAL_57600            ((FCY/57600)/BAUDRATE_DIV)-1
#define BRGVAL_115200           ((FCY/115200)/BAUDRATE_DIV)-1
#define SPECIAL_BAUDRATE        14400               /* if not on the list */
#define BRGVAL_SPECIAL          ((FCY/SPECIAL_BAUDRATE)/BAUDRATE_DIV)-1

/* UxMode PDSEL */
enum {
    NONE_PARITY_8BITS = 0,
    EVEN_PARITY_8BITS,
    ODD_PARITY_8BITS,
    NONE_PARITY_9BITS
};

extern int uart2dataFlag;
extern char uart2dataBuffer[256];
extern u8 uart2dataCounter;


/* function prototype */
void initUART2(unsigned long baudrate);
void writeUART2Byte(u8 data);
void writeUART2String(u8 *cmd) __attribute__ ((section (".libperi")));
void initUART2(unsigned long baudrate);
extern struct  STRUCT_USARTx_Fram                                  //??wifi??????????
{
    char  Data_RX_BUF [RX_BUF_MAX_LEN];         //RX_BUF_MAX_LEN

  union {
    u16 InfAll;
    struct {
          u8 FramLength       :7;                                    // 6:0 
          u8 FramFinishFlag   :1;                                   // 7 
      } InfBit;
  }; 

} strPc_Fram_Record, strEsp8266_Fram_Record;


extern struct  STRUCT_USART1_1_Fram                                   //??wifi??????????
{
    char  Data_RX_BUF [RX_BUF_MAX_LEN];            //RX_BUF_MAX_LEN

  union {
    u8 InfAll;
    struct {
          u8 FramLength       :7 ;                                   // 6:0 
          u8 FramFinishFlag   :1 ;                                   // 7 
      } InfBit;
  }; 
} strPc1_1_Fram_Record, str1_1esp8266;

#endif	/* XC_HEADER_TEMPLATE_H */

