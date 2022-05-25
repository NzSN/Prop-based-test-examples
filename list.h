#ifndef LIST_H
#define LIST_H

typedef void (*NodeDestructor)(void *val);
typedef void (*NodeDuplicate)(void *val);

typedef struct Node {
    struct Node *prev;
    struct Node *next;
    void *val;
} Node;

typedef struct List {
    Node head;
    Node *tail;
    /* If destructor is NULL then do nothing */
    NodeDestructor destructor;
    /* Only copyable when duplicate is not NULL */
    NodeDuplicate duplicate;
    int size;
} List;


/* Member function implement as macros */
#define listSize(L) ((L)->size)
#define listHead(L) ((L)->head.next)
#define listSetHead(L, N) ((L)->head.next = (N))
#define listTail(L) ((L)->tail)
#define listSetTail(L, N) ((L)->tail = (N))
#define listEmpty(L) ((L)->head.next == NULL)
#define listSetDestructor(L, D) ((L)->destructor=(D))
#define listSetDuplicate(L, Dup) ((L)->duplicate=(Dup))

#define nodePrev(N) ((N)->prev)
#define setNodePrev(NL, NR) ((NL)->prev = (NR))
#define nodeNext(N) ((N)->next)
#define setNodeNext(NL, NR) ((NL)->next = (NR))
#define destroyNodeVal(L, N) ((L)->destructor((N)->val))
#define duplicateNodeVal(L, N) ((L)->duplicate((N)->val))


/* Prototypes */
List* createList(void);
void  destroyList(List **l);
void  listPush(List *l, void *val);
void* listPop(List *l);
void* listGet(List *l, int idx);



#endif /* LIST_H */
