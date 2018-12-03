
#ifndef UART_H
#define	UART_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "defines.h"
#define UART_RX_BUFFER_SIZE             512
#define UART_TX_BUFFER_SIZE             512

#ifndef FCY
#define FCY 40000000
#endif

#define UART_LOW_SPEED_MODE         0
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
extern unsigned char uart2dataCounter;


/* function prototype */
void initUART2(unsigned long baudrate);
void writeUART2Byte(unsigned char data);
void writeUART2String(unsigned char *data, unsigned char length) __attribute__ ((section (".libperi")));


#define RX_BUF_MAX_LEN 50
typedef enum TT  {
    TCP,
    UDP    
};
typedef enum MODE {
    STA,
    AP,
    STA_AP
}MODE;
void initial_UART();
void ESP8266_Uart(char* cmd);
u8 ESP8266_Cmd( char* cmd , char* reply1 , char* reply2 , u16 waittime );

u8 ESP8266_SendString (u8 ucId , char* pStr , u16 ulStrLength);

void ESP8266_AT_Test ( void ) ;
void ESP8266_Rst ( void )  ;
u8 ESP8266_Net_Mode_Choose ( enum MODE MODE ) ;
u8 ESP8266_JoinAP ( char* pSSID, char* pPassWord );
u8 ESP8266_BuildAP ( char* pSSID, char* pPassWord, char* enunPsdMode );
u8 ESP8266_Link_Server ( enum TT enumE, char* ip, char* ComNum, int id);
void AP_MODE(void);
 void ESP8266_Server(void);

#endif	/* XC_HEADER_TEMPLATE_H */

