
#ifndef _DEFINES_H
#define	_DEFINES_H

#include <xc.h> // include processor files - each processor file is guarded.  
typedef unsigned int u16;
typedef unsigned char u8;
typedef unsigned char bool;
#define ADMax_minus_ADMin_300_up 139   //255-116
#define ADMax_minus_ADMin_300_down 83   //255-172
#define ADMin 116
#define Max_minus_Min_300_up 1235   //0x07B4-0x02E1
#define Max_minus_Min_300_down 497   //0x9a5-0x07B4
#define Max_minus_Min_300 1732   //0x09A5-0x02E1
#define Max_divide_Min 7.697841726618705
#define Minimum_300 737 //0x02E1
#define true 1
#define false 0

#endif	/* XC_HEADER_TEMPLATE_H */

