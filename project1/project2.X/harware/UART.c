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

void __attribute__((interrupt, auto_psv)) _U2RXInterrupt(void)
{
    char ch;
        ch  = U2RXREG ;
        if( strEsp8266_Fram_Record .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) ) {  //??1???????
            strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ++ ]  = ch;
            test[test_index++]=ch ;
#ifndef teacher 
            if(strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength] == '\0')
                strEsp8266_Fram_Record .InfBit .FramFinishFlag = 1;
#endif
        }
    IFS1bits.U2RXIF = 0;
}