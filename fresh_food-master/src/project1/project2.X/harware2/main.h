#ifndef _MAIN_H
#define	_MAIN_H

#define		CPU_FCY	40 				// for LCD elay referewnce
#ifndef Fcy
#define Fcy     CPU_FCY*1000000
#endif
#define Delay_200uS_Cnt  (Fcy * 0.02) / 1080

#endif	/* _MAIN_H */