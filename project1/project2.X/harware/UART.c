#include <xc.h> 
#include "UART.h"
#include <stdio.h>
#include <string.h>
#include <p33FJ128MC706A.h>
#include "defines.h"
#include "delay.h"

 //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern char test[10];
extern int test_index;
 int uart2dataFlag = 0;
char uart2dataBuffer[256] = {0};
u8 uart2dataCounter = 0;
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
} strPc1_1_Fram_Record, HMI_Fram_Record_1;


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
void initUART1(unsigned long baudrate)
{
    /* FIXME: check baudrate range */

    U1MODE = 0x0000; /* clear all setting */
    
    U1MODEbits.RTSMD = 1; //1 = UxRTS pin in Simplex mode; 0 = UxRTS pin in Flow Control mode
    U1MODEbits.PDSEL = NONE_PARITY_8BITS; //8-bit data,no parity
    U1MODEbits.STSEL = 0; //1 = Two Stop bits; 0 = One Stop bit
    U1MODEbits.BRGH=0;
    U1MODEbits.URXINV=0;
            
    U1STAbits.UTXISEL0 = 0; // Interrupt generated when any character is transferred to the Transmit Shift register
    U1STAbits.UTXISEL1 = 0;
    U1STAbits.URXISEL = 0; // Interrupt is set when any character is received and transferred from the UxRSR to the receive
    
    U1BRG = ((40000000 / baudrate) / BAUDRATE_DIV) - 1;
 
    
    TRISFbits.TRISF2 = 1; //U1 RX
    TRISFbits.TRISF3 = 0; //U1 TX

    IFS0bits.U1TXIF = 0; // Clear the Transmit Interrupt Flag
    IEC0bits.U1TXIE = 0; // Disable Transmit Interrupts
    IFS0bits.U1RXIF = 0; // Clear the Recieve Interrupt Flag
    IEC0bits.U1RXIE = 1; // enable U2RX interrupt

    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable UART Tx
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
u8 * UART_ReceiveString (  )
{
    char * pRecStr = 0;
    HMI_Fram_Record_1 .InfBit .FramLength = 0;
    if(  (u8)strstr ( HMI_Fram_Record_1 .Data_RX_BUF, "!" )){
        HMI_Fram_Record_1 .InfBit .FramLength = 0;
        HMI_Fram_Record_1 .InfBit .FramFinishFlag = 0;
        while ( ! HMI_Fram_Record_1 .InfBit .FramFinishFlag );
        if ( strstr ( HMI_Fram_Record_1 .Data_RX_BUF, "\0" ) )
           pRecStr = HMI_Fram_Record_1 .Data_RX_BUF;
    }
    else
        pRecStr=NULL;
    return pRecStr;
}
void __attribute__((interrupt, auto_psv)) _U2TXInterrupt(void)
{
    IFS1bits.U2TXIF = 0;
}

void __attribute__((interrupt, auto_psv)) _U2RXInterrupt(void)
{
        char ch;
        ch  = U2RXREG ;
        if( strEsp8266_Fram_Record .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) ) {  //??1???????
            strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ++ ]  = ch;
            test[test_index++]=ch ;   //debug用的
            
        if(strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength-1] == '\0')
           strEsp8266_Fram_Record .InfBit .FramFinishFlag = 1;
        }
    IFS1bits.U2RXIF = 0;
}
void __attribute__((interrupt, auto_psv)) _U1RXInterrupt(void)
{
        char ch;
        ch  = U1RXREG ;
        if( HMI_Fram_Record_1 .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) ) {  //??1???????
            HMI_Fram_Record_1 .Data_RX_BUF [ HMI_Fram_Record_1 .InfBit .FramLength ++ ]  = ch;
            test[test_index++]=ch ;   //debug用的
            
        if(HMI_Fram_Record_1 .Data_RX_BUF [ HMI_Fram_Record_1 .InfBit .FramLength-1] == '\0')
           HMI_Fram_Record_1 .InfBit .FramFinishFlag = 1;
        }
    IFS0bits.U1RXIF = 0;
}