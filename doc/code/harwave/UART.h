
#ifndef UART_H
#define	UART_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "defines.h"
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

