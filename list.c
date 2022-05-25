#include <string.h>
#include <malloc.h>
#include "list.h"


List* createList() {
    List *l = (List*)malloc(sizeof(List));
    memset(l, 0, sizeof(List));

    return l;
}

void destroyList(List **l_) {

    List *l = *l_;
    Node *n = listTail(l), *next;

    while (n) {
        next = nodePrev(n);

        if (l->destructor) {
            destroyNodeVal(l, n);
            free(n);
        }

        n = next;
    }

    free(l);
    *l_ = NULL;
}

void listPush(List *l, void *val) {

    Node *n = (Node*)malloc(sizeof(Node));
    n->val = val;
    n->prev = NULL;
    n->next = NULL;

    if (listEmpty(l)) {
        listSetHead(l, n);
        listSetTail(l, n);
    } else {
        setNodeNext(listTail(l), n);
        setNodePrev(n, listTail(l));
        listSetTail(l, n);
    }

    l->size++;
}

void* listPop(List *l) {
    Node *n = listHead(l);

    if (n == NULL) {
        return n;
    }

    listSetHead(l, n->next);
    if (listEmpty(l)) {
        listSetTail(l, NULL);
    } else {
        setNodePrev(listHead(l), NULL);
    }

    void *v = n->val;
    free(n);

    l->size--;

    return v;
}

void* listGet(List *l, int idx) {

    int i = 0;

    if (listEmpty(l))
        return NULL;

    Node *current = listHead(l);
    while (current && ++i <= idx)
        current = nodeNext(current);

    return current->val;
}
