#include "replace.h"
#include <xc.h>
 u8 debounce_register;
void debounce(u8 a){
    debounce_register=a;
    if( !T2CONbits.TMR2ON )
        T2CONbits.TMR2ON=1;    
}
