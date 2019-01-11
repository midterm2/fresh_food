
#ifndef _LinkingList_H
#define	_LinkingList_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "defines.h"
#include<stdio.h>
#include<stdlib.h>
typedef struct listNode* listPointer;

typedef struct listNode {
    char data[5] ;
    listPointer link;
    listPointer head;
}Node;


listPointer create (u8* data);
void insert(listPointer* first,u8* data);
void Delete(listPointer* first,listPointer x,listPointer trail);
#endif	/* XC_HEADER_TEMPLATE_H */

