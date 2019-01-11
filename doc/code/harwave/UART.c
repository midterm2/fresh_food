#include <xc.h> 
#include "UART.h"
#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "mydelay.h"
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
void initial_UART(){                                             //9600 baud rate ???
    TXSTAbits.SYNC=0;      //Asynchronous mode
    TXSTAbits.TX9=0;       //8-bit transmission
    RCSTAbits.SPEN=1;      //Serial Port Enable bit
    PIE1bits.TX1IE=0;      //turn off TX interrupt
    TXSTAbits.TXEN=1;         //enable the transmission  TXIF=1->TXREG register is empty
    
    RCONbits.IPEN=1;                    // turn off priority level
    INTCONbits.GIE=1;                   // Peripheral Interrupt disEnable bit
    INTCONbits.PEIE=1;                  // Global Interrupt Enable bit
    
    RCSTAbits.CREN=1;
    PIE1bits.RCIE=1;       //enable RX interrupt
    RCSTAbits.RC9 = 0;     //8-bit reception
    TXSTAbits.BRGH=0;      //Low speed Baud Rate
    BAUDCONbits.BRG16=0;
    SPBRGH=0;
    SPBRG=64;
}
void ESP8266_Uart(char* cmd){                                   //transmit cmd
    int index=-1;
//        TXSTAbits.TXEN=1;         //enable the transmission  TXIF=1->TXREG register is empty
    do
    {
        index++;
        while(!TXIF);         //waitting for TXREG = empty
        TXREG=cmd[index];
    }while(cmd[index]!='\0');
    while(!TRMT);
//    TXSTAbits.TXEN=0;        //disable the transmission
}
u8 ESP8266_Cmd ( char* cmd , char* reply1 , char* reply2 ,u16 waittime )              // ESP8266 command instruction
    {    
        strEsp8266_Fram_Record .InfBit .FramLength = 0;               //???????????

        ESP8266_Uart (cmd);
        
        if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                      //???????
            return true;

        delay_ms ( waittime );                 //????????????


        if ( ( reply1 != 0 ) && ( reply2 != 0 ) )
            return (  !strcmp ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) || !strcmp ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) ); 
        else if ( reply1 != 0 )
            return (  !strcmp ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) );

        else
            return ( !strcmp ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) );


    }   
 u8 ESP8266_SendString ( u8 ucId ,char* pStr, u16 ulStrLength)  // ucId ????   send String 
{
    char cStr [20];
    int bRet = false;

    sprintf ( cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2 );

    ESP8266_Cmd ( cStr,"> ",0, 1000 ) ;

    bRet = ESP8266_Cmd ( pStr, "SEND OK", 0, 1000 );

    return bRet;

}
 void ESP8266_AT_Test ( void )          //AT??
{

    delay_ms ( 1000 ); 

    while ( ! ESP8266_Cmd ( "AT", "OK", NULL, 200 ) ) ;     

}
 void ESP8266_Rst ( void )            //AT reset
{
    #if 1
     ESP8266_Cmd ( "AT+RST", "OK", "ready", 2500 );     

    #else
     ESP8266_RST_LOW_LEVEL();
     Delay_ms ( 500 ); 
     ESP8266_RST_HIGH_LEVEL();

    #endif

}
u8 ESP8266_Net_Mode_Choose ( enum MODE MODE )   // STA AP STA/AP
{
    
    switch ( MODE )
    {
        case STA:
            return ESP8266_Cmd ( "AT+CWMODE=1", "OK", 0, 2500 ); 

        case AP:
          return ESP8266_Cmd ( "AT+CWMODE=2", "OK", 0, 2500 ); 

        case STA_AP:
          return ESP8266_Cmd ( "AT+CWMODE=3", "OK", 0, 2500 ); 

      default:
          return false;
  }

}
u8 ESP8266_JoinAP ( char* pSSID, char* pPassWord )             // to connect AP
{
    char cCmd [120];
 
    sprintf ( cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );

    return ESP8266_Cmd ( cCmd, "OK", NULL, 7000 );

}
u8 ESP8266_BuildAP ( char* pSSID, char* pPassWord, char* enunPsdMode )        //pSSID name of AP pPassWord possword of AP enunPsdMode  method of encrypt   //channel =1 (default))
{
    char cCmd [120];

    sprintf ( cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%s", pSSID, pPassWord, enunPsdMode );

    return ESP8266_Cmd ( cCmd, "OK", 0, 1000 );

}
u8 ESP8266_Link_Server ( enum TT enumE, char* ip, char* ComNum, int id) //id:0 is able to client/server contact?the other id only for server  //enumE?TCP?/?UDP?//  ip server IP address   //ComNum port number
{
    char cStr [100] = { 0 }, cCmd [120];

  switch (  enumE )
  {
        case TCP:
          sprintf ( cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum );
          break;

        case UDP:
          sprintf ( cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum );
          break;

        default:
            break;
  }

  if ( id < 5 ) //CIPMUX=1
    sprintf ( cCmd, "AT+CIPSTART=%d,%s", id, cStr);

  else  //CIPMUX=0
      sprintf ( cCmd, "AT+CIPSTART=%s", cStr );

    return ESP8266_Cmd ( cCmd, "OK", "ALREAY CONNECT", 500 );

}
void AP_MODE(){
    ESP8266_Net_Mode_Choose ( AP ) ;
    while(!ESP8266_Cmd ( "AT+CWMODE?", "+CWMODE:2" , "OK" ,1000));
}
 void ESP8266_Server(){
    ESP8266_AT_Test () ; 
    AP_MODE() ;
    ESP8266_BuildAP ( "KENG", "12345678", "4"); 
    while(!ESP8266_Cmd ( "AT+CIPMODE=0", "OK", 0 , 500 ));
    while(!ESP8266_Cmd ( "AT+CIPMUX=1", "OK", 0 , 500 ));
    while(!ESP8266_Cmd ( "AT+CIPSERVER=1,5000", "OK", 0 , 500 ));
}