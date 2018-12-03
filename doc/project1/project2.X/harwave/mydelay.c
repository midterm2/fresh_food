//#include <delays.h>
#include"mydelay.h"
void delay_ms(int ms)
{
    do{
//        Delay10KTCYx(1);    
        ms--;   
    }while(ms);
}

void delay_us(int us)
{
    do{
//        Delay10TCYx(1);
        us--;
    }while(us);
}
void delayFor18Tcy(void)
{
//    Delay1TCYx(18);
}
