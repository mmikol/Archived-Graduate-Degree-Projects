// Author: Miliano Mikol (mmikol@usc.edu)

#include <stdlib.h>
#include "cs402.h"
#include "my402list.h"

int  My402ListLength(My402List* ll) {
    return ll->num_members;
};

int  My402ListEmpty(My402List* ll) {
    return ll->num_members <= 0;
};

int  My402ListAppend(My402List* ll, void* obj) {
    My402ListElem *elem=(My402ListElem*)malloc(sizeof(My402ListElem));

    if (!elem) return 0;

    My402ListElem *last=My402ListLast(ll);

    elem->obj=obj;
    elem->next=&ll->anchor;
    ll->anchor.prev=elem;

    if (!last) {
        elem->prev=&ll->anchor;
        ll->anchor.next=elem;
    } else {
        elem->prev=last;
        last->next=elem;
    }

    ll->num_members++;

    return 1;
};

int  My402ListPrepend(My402List* ll, void* obj) {
    My402ListElem *elem=(My402ListElem*)malloc(sizeof(My402ListElem));

    if (!elem) return 0;

    My402ListElem *first=My402ListFirst(ll);

    elem->obj=obj;
    elem->prev=&ll->anchor;
    ll->anchor.next=elem;

    if (!first) {
        elem->next=&ll->anchor;
        ll->anchor.prev=elem;
    } else {
        elem->next=first;
        first->prev=elem;
    }

    ll->num_members++;

    return 1;
};

void My402ListUnlink(My402List* ll, My402ListElem* elem) {
    if (!My402ListEmpty(ll)) {
        elem->prev->next=elem->next;
        elem->next->prev=elem->prev;
        ll->num_members--;
        free(elem);        
    }
};

void My402ListUnlinkAll(My402List* ll) {
    if (!My402ListEmpty(ll)) {
        My402ListElem* elem=My402ListFirst(ll);
        while (elem) {
            My402ListElem* remove=elem;
            elem=My402ListNext(ll, elem);
            remove->prev->next=remove->next;
            remove->next->prev=remove->prev;
            ll->num_members--;
            free(remove);   
        }
    }
};

int  My402ListInsertAfter(My402List* ll, void* obj, My402ListElem* elem) {
    My402ListElem *new=(My402ListElem*)malloc(sizeof(My402ListElem));
        
    if (!new) return 0;
    
    new->obj=obj;
    new->next=elem->next;
    new->prev=elem;
    elem->next->prev=new;
    elem->next=new;
    ll->num_members++;

    return 1;
};

int  My402ListInsertBefore(My402List* ll, void* obj, My402ListElem* elem) {
    My402ListElem *new=(My402ListElem*)malloc(sizeof(My402ListElem));
        
    if (!new) return 0;

    new->obj=obj;
    new->next=elem;
    new->prev=elem->prev;
    elem->prev->next=new;
    elem->prev=new;
    ll->num_members++;

    return 1;
};

My402ListElem* My402ListFirst(My402List* ll) {
    return ll->num_members > 0 ? ll->anchor.next : NULL;
};

My402ListElem* My402ListLast(My402List* ll) {
    return ll->num_members > 0 ? ll->anchor.prev : NULL;
}

My402ListElem *My402ListNext(My402List* ll, My402ListElem* elem) {
    return elem != ll->anchor.prev ? elem->next : NULL;
};

My402ListElem *My402ListPrev(My402List* ll, My402ListElem* elem) {
    return elem != ll->anchor.next ? elem->prev : NULL;
};

My402ListElem *My402ListFind(My402List* ll, void* obj) {
    for (My402ListElem *elem=My402ListFirst(ll); elem != NULL; elem=My402ListNext(ll, elem)) {
        if (elem->obj == obj) return elem;
    }
    return NULL;
}

int My402ListInit(My402List* ll)  {
    ll->num_members=0;
    ll->anchor.obj=NULL;
    ll->anchor.next=&ll->anchor;
    ll->anchor.prev=&ll->anchor;
    return 1;
};