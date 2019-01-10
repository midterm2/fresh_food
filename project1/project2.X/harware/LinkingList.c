#include "LinkingList.h"
#include <stdio.h>
#include <stdlib.h>
listPointer create (u8* data){
    listPointer first;
    first=(listPointer)malloc(sizeof(struct listNode));
    first->data[0]=*(data);
    first->data[1]=*(data+1);
    first->data[2]=*(data+2);
    first->data[3]=*(data+3);
    first->data[4]=*(data+4);
    first->link=NULL;
    return first;
}
void insert(listPointer* first,u8* data){
    listPointer a,new;
    new=create(data);
    if(*first){
       for( a=*first ; a->link != NULL ; a=a->link); 
       a->link=create(data);
       a->link->head=a;
    }
    else{
        (*first)=new;
        (*first)->head=NULL;
    }
}
void Delete(listPointer* first,listPointer x,listPointer trail){
    if(trail)
        trail->link=x->link;
    else
        *first=x->link;
    free(x);
}