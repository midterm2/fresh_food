
#ifndef _DELAY_H
#define	_DELAY_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <libpic30.h>
#define FCY 40000000UL
#if !defined(FCY)
extern void __delay_ms(unsigned long);
extern void __delay_us(unsigned long);
#else
#define __delay_ms(d) \
    { __delay32( (unsigned long) (((unsigned long long) d)*(FCY)/1000ULL)); }
#define __delay_us(d) \
  { __delay32( (unsigned long) (((unsigned long long) d)*(FCY)/1000000ULL)); }
#endif
#define TCY (float)1/40000000
#endif	

