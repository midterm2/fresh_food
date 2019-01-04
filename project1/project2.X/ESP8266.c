#include "esp8266.h"
#include "defines.h"
#include <string.h>
#include "delay.h"
#include <stdio.h>
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
 u8 ESP8266_SendString (  char* pStr, u16 ulStrLength)  // ucId ????   send String 
{
    char cStr [20];
    int bRet = false;

    sprintf ( cStr, "AT+CIPSEND=%d\r\n", ulStrLength );

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
    while(!ESP8266_JoinAP( (void*)"EE501" , (void*)"10993042" ));
    while(!ESP8266_Cmd ( (void *)"AT+CIPMODE=1\r\n", (void *)"OK", 0 , 500 ));
    while(!ESP8266_Cmd ( (void *)"AT+CIPMUX=0\r\n", (void *)"OK", 0 , 500 ));
 }
 void SEND_URL(char* value1,char* value2,char* value3){
     while(!ESP8266_Link_Server(TCP,(void*)"maker.ifttt.com",(void*)"80",7));
     char* url;
     int length;
     sprintf(url,"GET /trigger/line/key/cOYg0W-vkyDZ6ZeR4cbdcY?value1=24&value2=37 HTTP/1.1\r\nHost: maker.ifttt.com\r\n\r\n");
     length=strlen(url);
     ESP8266_SendString(url,length);
     
 }
