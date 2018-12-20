#include <xc.h> 
#include "UART.h"
#include <stdio.h>
#include <string.h>
#include <p33FJ128MC706A.h>
#include "defines.h"
#include "delay.h"
char test[10];
int test_index;
struct  STRUCT_USARTx_Fram                                  //??wifi??????????
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


struct  STRUCT_USART1_1_Fram                                   //??wifi??????????
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
#ifndef teacher
void ESP8266_Uart(char* cmd){                                   //transmit cmd
    int index=-1;
    do
    {
        index++;
        writeUART2Byte(cmd[index]);
    }while(cmd[index]!='\0');
}
#endif
u8 ESP8266_Cmd ( char* cmd , char* reply1 , char* reply2 ,u16 waittime )              // ESP8266 command instruction
    {    
        strEsp8266_Fram_Record .InfBit .FramLength = 0;               //???????????
        test_index=0;
#ifdef teacher
        writeUART2String((u8*)cmd);
#else
        ESP8266_Uart(cmd) ;
#endif
        if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                      //???????
            return true;
        __delay_ms ( waittime );                 //????????????
#ifdef teacher        
        strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';

        strEsp8266_Fram_Record .InfBit .FramFinishFlag = 1;
#endif

        if ( ( reply1 != 0 ) && ( reply2 != 0 ) )
            return (   (unsigned short)strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) ||  (unsigned short)strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) ); 
        else if ( reply1 != 0 )
            return (   (unsigned short)strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) );
        else
            return (  (unsigned short)strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) );


    }   
 u8 ESP8266_SendString ( u8 ucId ,char* pStr, u16 ulStrLength)  // ucId ????   send String 
{
    char cStr [20];
    int bRet = false;

    sprintf ( cStr, "AT+CIPSEND=%d,%d\r\n", ucId, ulStrLength + 2 );

    ESP8266_Cmd ( cStr,"> ",0, 1000 ) ;

    bRet = ESP8266_Cmd ( pStr, "SEND OK", 0, 1000 );

    return bRet;

}
 void ESP8266_AT_Test ( void )          //AT??
{
    while ( ! ESP8266_Cmd ( (void*)"AT\r\n", (void*)"OK" , 0 , 1000 ) ) ;     

}
 void ESP8266_Rst ( void )            //AT reset
{
    #if 1
     ESP8266_Cmd ( "AT+RST\r\n", "OK", "ready", 3500 );     

    #else
     ESP8266_RST_LOW_LEVEL();
     __delay_ms ( 500 ); 
     ESP8266_RST_HIGH_LEVEL();

    #endif

}
u8 ESP8266_Net_Mode_Choose ( enum MODE MODE )   // STA AP STA/AP
{
    
    switch ( MODE )
    {
        case STA:
            return ESP8266_Cmd ( "AT+CWMODE=1\r\n", "OK", 0, 2500 ); 

        case AP:
          return ESP8266_Cmd ( "AT+CWMODE=2\r\n", "OK", 0, 2500 ); 

        case STA_AP:
          return ESP8266_Cmd ( "AT+CWMODE=3\r\n", "OK", 0, 2500 ); 

      default:
          return false;
  }

}
u8 ESP8266_JoinAP ( char* pSSID, char* pPassWord )             // to connect AP
{
    char cCmd [120];
 
    sprintf ( cCmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", pSSID, pPassWord );

    return ESP8266_Cmd ( cCmd, "OK", "CONNECTED", 7000 );

}
u8 ESP8266_BuildAP ( char* pSSID, char* pPassWord, char* enunPsdMode )        //pSSID name of AP pPassWord possword of AP enunPsdMode  method of encrypt   //channel =1 (default))
{
    char cCmd [120];

    sprintf ( cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%s\r\n", pSSID, pPassWord, enunPsdMode );

    return ESP8266_Cmd ( cCmd, "OK", 0, 1000 );

}
u8 ESP8266_Link_Server ( enum TT enumE, char* ip, char* ComNum, int id) //id:0 is able to client/server contact?the other id only for server  //enumE?TCP?/?UDP?//  ip server IP address   //ComNum port number
{
    char cStr [100] = { 0 }, cCmd [120];

  switch (  enumE )
  {
        case TCP:
          sprintf ( cStr, "\"%s\",\"%s\",%s\r\n", "TCP", ip, ComNum );
          break;

        case UDP:
          sprintf ( cStr, "\"%s\",\"%s\",%s\r\n", "UDP", ip, ComNum );
          break;

        default:
            break;
  }

  if ( id < 5 ) //CIPMUX=1
    sprintf ( cCmd, "AT+CIPSTART=%d,%s\r\n", id, cStr);

  else  //CIPMUX=0
      sprintf ( cCmd, "AT+CIPSTART=%s\r\n", cStr );

    return ESP8266_Cmd ( cCmd, "OK", "ALREAY CONNECT", 500 );

}
void AP_MODE(){
    ESP8266_Net_Mode_Choose ( AP ) ;
    while(!ESP8266_Cmd ( "AT+CWMODE?\r\n", "+CWMODE:2" , "OK" ,1000));
}
void STA_MODE(){
    ESP8266_Net_Mode_Choose ( STA ) ;
    while(!ESP8266_Cmd ( (void*)"AT+CWMODE?\r\n", (void*)"+CWMODE:1" , (void*)"OK" ,100));
}
 void ESP8266_Server(){
    ESP8266_AT_Test () ; 
    ESP8266_Rst (  );
    AP_MODE();
    while(!ESP8266_BuildAP ( (void *)"KEN", (void *)"zx4522331", (void *)"4")); 
    while(!ESP8266_Cmd ( (void *)"AT+CIPMUX=0\r\n",(void *) "OK", 0 , 500 ));
    while(!ESP8266_Cmd ( (void *)"AT+CIPMODE=0\r\n", (void *)"OK", 0 , 1000 ));
//    while(!ESP8266_Cmd ( "AT+CIPSERVER=0\r\n", "OK", 0 , 800 ));
    while(!ESP8266_Cmd ( (void *)"AT+CIPMUX=1\r\n", (void *)"OK", 0 , 500 ));
    while(!ESP8266_Cmd ( (void *)"AT+CIPSERVER=1,5000\r\n", (void *)"OK", 0 , 500 ));                //port location 5000
}
 void ESP8266_client(){
    ESP8266_Rst ();
    ESP8266_AT_Test () ;
    ESP8266_Cmd ( (void *)"AT+CWQAP\r\n",(void *)"OK", 0 , 500 );
    STA_MODE();
    while(!ESP8266_JoinAP( (void*)"Keng" , (void*)"12345675" ));
    while(!ESP8266_Cmd ( (void *)"AT+CIPMODE=1\r\n", (void *)"OK", 0 , 500 ));
    while(!ESP8266_Cmd ( (void *)"AT+CIPMUX=0\r\n", (void *)"OK", 0 , 500 ));
    while(!ESP8266_Link_Server(TCP,(void*)"maker.ifttt.com",(void*)"80",7));    //?????>5???????AT+CIPMUX=0
 }
 //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 
 int uart2dataFlag = 0;
char uart2dataBuffer[256] = {0};
u8 uart2dataCounter = 0;

void initUART2(unsigned long baudrate)
{
    /* FIXME: check baudrate range */

    U2MODE = 0x0000; /* clear all setting */
    
    U2MODEbits.RTSMD = 1; //1 = UxRTS pin in Simplex mode; 0 = UxRTS pin in Flow Control mode
    U2MODEbits.PDSEL = NONE_PARITY_8BITS; //8-bit data,no parity
    U2MODEbits.STSEL = 0; //1 = Two Stop bits; 0 = One Stop bit
    U2MODEbits.BRGH=0;
    U2MODEbits.URXINV=0;
            
    U2STAbits.UTXISEL0 = 0; // Interrupt generated when any character is transferred to the Transmit Shift register
    U2STAbits.UTXISEL1 = 0;
    U2STAbits.URXISEL = 0; // Interrupt is set when any character is received and transferred from the UxRSR to the receive
    
    U2BRG = ((40000000 / baudrate) / BAUDRATE_DIV) - 1;
 
    
    TRISFbits.TRISF4 = 1; //U2 RX
    TRISFbits.TRISF5 = 0; //U2 TX

    IFS1bits.U2TXIF = 0; // Clear the Transmit Interrupt Flag
    IEC1bits.U2TXIE = 0; // Disable Transmit Interrupts
    IFS1bits.U2RXIF = 0; // Clear the Recieve Interrupt Flag
    IEC1bits.U2RXIE = 1; // enable U2RX interrupt

    U2MODEbits.UARTEN = 1; // Enable UART
    U2STAbits.UTXEN = 1; // Enable UART Tx
}

void writeUART2Byte(u8 data)
{
    while (U2STAbits.UTXBF); /* wait if the buffer is full */
    U2TXREG = data;
}
#ifdef teacher
void writeUART2String(u8 *cmd)
{
    int length;
    length=strlen((const char*)cmd);
    int i;
    for (i = 0; i < length; i++) {
        writeUART2Byte(cmd[i]);
    }
}
#endif
void __attribute__((interrupt, auto_psv)) _U2TXInterrupt(void)
{
    IFS1bits.U2TXIF = 0;
}

