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
void ESP8266_Uart(char* cmd){                                   //transmit cmd
    int index=-1;
    do
    {
        index++;
//        while(!TXIF);         //waitting for TXREG = empty
//        TXREG=cmd[index];
    }while(cmd[index]!='\0');
//    while(!TRMT);
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
 //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 
 int uart2dataFlag = 0;
char uart2dataBuffer[256] = {0};
unsigned char uart2dataCounter = 0;

void initUART2(unsigned long baudrate)
{
    /* FIXME: check baudrate range */

    U2MODE = 0x0000; /* clear all setting */
    U2MODEbits.RTSMD = 1; //1 = UxRTS pin in Simplex mode; 0 = UxRTS pin in Flow Control mode
    U2MODEbits.PDSEL = ODD_PARITY_8BITS; //8-bit data,no parity
    U2MODEbits.STSEL = 0; //1 = Two Stop bits; 0 = One Stop bit
    U2MODEbits.BRGH=0;
    
    U2STAbits.UTXISEL0 = 0; // Interrupt generated when any character is transferred to the Transmit Shift register
    U2STAbits.UTXISEL1 = 0;
    U2STAbits.URXISEL = 0; // Interrupt is set when any character is received and transferred from the UxRSR to the receive

    U2BRG = ((FCY / baudrate) / BAUDRATE_DIV) - 1;
 
    
    TRISFbits.TRISF4 = 1; //U2 RX
    TRISFbits.TRISF5 = 0; //U2 TX

    IFS1bits.U2TXIF = 0; // Clear the Transmit Interrupt Flag
    IEC1bits.U2TXIE = 0; // Disable Transmit Interrupts
    IFS1bits.U2RXIF = 0; // Clear the Recieve Interrupt Flag
    IEC1bits.U2RXIE = 1; // enable U2RX interrupt

    U2MODEbits.UARTEN = 1; // Enable UART
    U2STAbits.UTXEN = 1; // Enable UART Tx
}

void writeUART2Byte(unsigned char data)
{
    while (U2STAbits.UTXBF); /* wait if the buffer is full */
    U2TXREG = data;
}

void writeUART2String(unsigned char *data, unsigned char length)
{
    int i;
    for (i = 0; i < length; i++) {
        writeUART2Byte(data[i]);
    }
}

void __attribute__((interrupt, auto_psv)) _U2TXInterrupt(void)
{
    IFS1bits.U2TXIF = 0;
}

void __attribute__((interrupt, auto_psv)) _U2RXInterrupt(void)
{
    int i=0;
    IFS1bits.U2RXIF = 0;
    uart2dataFlag = 1;
    uart2dataBuffer[uart2dataCounter++] = U2RXREG;
}