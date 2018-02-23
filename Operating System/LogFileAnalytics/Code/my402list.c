#include "cs402.h"
#include "my402list.h"
#include <stdlib.h>
#include <sys/time.h>


int  My402ListLength(My402List* mylist)
{
    return mylist->num_members;
}

int  My402ListEmpty(My402List* mylist)
{
    if(mylist->num_members==0)
        return TRUE;
    else
        return FALSE;
}

int  My402ListAppend(My402List* mylist, void* newObj)
{
    My402ListElem *element=(My402ListElem *)malloc(sizeof(My402ListElem));
    if(element!=NULL) {
        element->next= NULL;
        element->prev=NULL;
        element->obj=newObj;
        if (mylist->num_members == 0)
        {
            mylist->num_members = 1;
            element->next=&(mylist->anchor);
            element->prev=&(mylist->anchor);
            mylist->anchor.prev=element;

          mylist->anchor.next=element;
        }
        else
        {
            mylist->num_members=mylist->num_members+1;
            element->next=&(mylist->anchor);
            element->prev=mylist->anchor.prev;
            mylist->anchor.prev=element;
            element->prev->next=element;


        }
        return TRUE;
    }
    else
        return FALSE;
}




int My402ListPrepend(My402List* mylist, void* obj)
{
    My402ListElem *element=(My402ListElem *)malloc(sizeof(My402ListElem));
    if(element!=NULL) {
        element->next= NULL;
        element->prev=NULL;
        element->obj=obj;
        if (mylist->num_members == 0)
        {
            mylist->num_members = 1;
            element->next=&(mylist->anchor);
            element->prev=&(mylist->anchor);
            mylist->anchor.prev=element;
            mylist->anchor.next=element;
        }
        else
        {
            mylist->num_members=mylist->num_members+1;
            element->next=mylist->anchor.next;
            element->prev=&(mylist->anchor);
            mylist->anchor.next=element;
            element->next->prev=element;
        }
        return TRUE;
    }
    else
        return FALSE;

}

void My402ListUnlink(My402List *mylist, My402ListElem *element)
{

    element->prev->next=element->next;
    element->next->prev=element->prev;
    element->next=NULL;
    element->prev=NULL;
    element->obj=NULL;
    free(element);
    mylist->num_members=mylist->num_members-1;
}

void My402ListUnlinkAll(My402List *mylist)
{
    My402ListElem *temp=NULL;
    while(mylist->anchor.next!=NULL)
    {
        temp=mylist->anchor.next;
        temp->prev->next=temp->next;
        temp->next->prev=temp->prev;
        temp->next=NULL;
        temp->prev=NULL;
        temp->obj=NULL;
        free(temp);
        mylist->num_members=mylist->num_members-1;
    }

}

int  My402ListInsertAfter(My402List* mylist, void *obj, My402ListElem *elem)
{
    My402ListElem *element=(My402ListElem *)malloc(sizeof(My402ListElem));
    if(element!=NULL) {
        element->next = NULL;
        element->prev = NULL;
        element->obj = obj;
        element->next=elem->next;
        element->prev=elem;
        elem->next=element;
        element->next->prev=element;
        mylist->num_members=mylist->num_members+1;
        return TRUE;

    }
    else
        return FALSE;

}

int  My402ListInsertBefore(My402List* mylist, void *obj, My402ListElem *elem)
{
    My402ListElem *element=(My402ListElem *)malloc(sizeof(My402ListElem));
    if(element!=NULL) {
        element->next = NULL;
        element->prev = NULL;
        element->obj = obj;
        element->next=elem;
        element->prev=elem->prev;
        element->prev->next=element;
        elem->prev=element;
        mylist->num_members=mylist->num_members+1;
        return TRUE;

    }
    else
        return FALSE;

}

My402ListElem *My402ListFirst(My402List* mylist)
{

    if(mylist->num_members==0)
        return NULL;
    else
        return mylist->anchor.next;
}

My402ListElem *My402ListLast(My402List *mylist)
{
    if(mylist->num_members==0)
        return NULL;
    else
        return mylist->anchor.prev;

}

My402ListElem *My402ListNext(My402List *mylist, My402ListElem *elem)
{
    if(elem->next==&(mylist->anchor))
        return NULL;
    else
        return elem->next;

}
My402ListElem *My402ListPrev(My402List *mylist, My402ListElem *elem)
{
    if(elem->prev==&(mylist->anchor))
        return NULL;
    else
        return elem->prev;

}

My402ListElem *My402ListFind(My402List *mylist, void *searchObj)
{
    My402ListElem *temp=NULL;
    temp=mylist->anchor.next;
    while(temp!=&(mylist->anchor))
    {
        if(temp->obj==searchObj)
            return temp;
        else
            temp=temp->next;
    }

    return NULL;

}

int My402ListInit(My402List* mylist)
{
        mylist->num_members=0;
        mylist->anchor.next=&(mylist->anchor);
        mylist->anchor.prev=&(mylist->anchor);
        mylist->anchor.obj=NULL;
        return TRUE;
}

