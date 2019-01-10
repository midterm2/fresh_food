/*
 * File:   main_fresh.c
 * Author: Keng
 *
 * Created on 2018年11月30日, 上午 10:55
 */


#include <xc.h>
#include <stdio.h>
#include "main.h"
#include "UART.h"
#include "defines.h"
#include "delay.h"
#include "esp8266.h"
#include "LinkingList.h"
/****************** CRITICAL SETUP, MODIFY THEM CAREFULLY *******************/
/* 0xF80000 FBS */
#pragma config BWRP = WRPROTECT_OFF, BSS = NO_BOOT_CODE, RBS = NO_BOOT_RAM
/* 0xF80002 FFS */
#pragma config SWRP = WRPROTECT_OFF, SSS = NO_SEC_CODE, RSS = NO_SEC_RAM
/* 0xF80004 FGS */
#pragma config GWRP = ON, GSS = HIGH
/* 0xF80006 FOSCSEL */
#pragma config IESO = ON, FNOSC = PRIPLL
/* 0xF80008 FOSCL */
#pragma config FCKSM = CSDCMD, OSCIOFNC = OFF, POSCMD = HS
/* 0xF8000A FWDT */
#pragma config WDTPOST = PS4096, WDTPRE = PR128, WINDIS = OFF, FWDTEN = OFF
/* 0xF8000C FPOR */
#pragma config PWMPIN = ON, HPOL = ON, LPOL = ON, FPWRT = PWR128
/* 0xF8000E FICD */
#pragma config JTAGEN = OFF, ICS = PGD2
/****************************************************************************/
extern char test[15];
extern int test_index;

void initPLL(void)//Fcy =40MHz;
{
    int i, j;

    CLKDIVbits.ROI = 0;
    CLKDIVbits.DOZEN = 0;
    CLKDIVbits.DOZE = 0;

    CLKDIVbits.PLLPRE = 18; // N1 = PLLPRE + 2 = 18+2 =20
    PLLFBD = 158; // M = 158+2=160
    CLKDIVbits.PLLPOST = 0; // N2 = 2           sum: Fosc= {OSC*M/(N1*N2)}=20M*160/(20*2)=80MHz
    // Tcy=1/80M*4=50nS
    SRbits.IPL = 0;

    //wait for Fosc stablize
    for (i = 1000; i > 0; i--)
        for (j = 1000; j > 0; j--);

}
void PORTS_Test_Initial(void)
{
    PORTB = 0xffff;
    TRISB = 0xffff;
    PORTB = 0xffff;

    PORTC = 0xffff;
    TRISC = 0x0000;
    PORTC = 0xffff;

    PORTD = 0xffff;
    TRISD = 0x0000;
    PORTD = 0xffff;

    PORTE = 0xffff;
    TRISE = 0x0000;
    PORTE = 0xffff;

    PORTF = 0xffff;
    TRISF = 0x0000;
    PORTF = 0xffff;

    PORTG = 0xffff;
    TRISG = 0x0000;
    PORTG = 0xffff;
}

void main(void)
{
 
    initPLL();
    while (OSCCONbits.CF == 1); //check clock failed
    while (OSCCONbits.COSC != 0b011); // Wait for Clock switch to occur
    while (OSCCONbits.LOCK == 0); //check PLL locked
    PORTS_Test_Initial();
    LATDbits.LATD11 = 1; /* turn the LCM back light */
    __delay_ms ( 5000 );  
    LATDbits.LATD11 = 0;
    initUART2(115200);
    ESP8266_client();
    SEND_URL("111","222","333");
    u8* item;
    listPointer first=NULL;
    while (1) {
        item=UART_ReceiveString ();
        if(item != NULL)
          insert(&first,item);
    }
}
