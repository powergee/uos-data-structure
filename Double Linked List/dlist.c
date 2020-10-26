#include <stdio.h>
#include <stdlib.h>

typedef int element;
typedef struct _DListNode {
    element data;
    struct _DListNode* prev;
    struct _DListNode* next;
} DListNode;

void init(DListNode* head)
{
    head->prev = head;
    head->next = head;
}

void dinsert(DListNode* before, element data)
{
    DListNode* newNode;

    newNode = (DListNode*)malloc(sizeof(DListNode));
    newNode->data = data;
    newNode->prev = before;
    newNode->next = before->next;

    before->next->prev = newNode;
    before->next = newNode;
}

void ddelete(DListNode* head, DListNode* removed)
{
    if (removed == head)
        return;
    
    removed->prev->next = removed->next;
    removed->next->prev = removed->prev;

    free(removed);
}

DListNode* search(DListNode* head, element data)
{
    DListNode* p;

    for (p = head->next; p != head; p = p->next)
        if (p->data == data)
            return p;
    return NULL;
}

void print_dlist(DListNode* head)
{
    DListNode* p;

    for (p = head->next; p != head; p = p->next)
        printf("<- |%d| ->", p->data);
    printf("\n");
}

int main()
{
    DListNode* head;
    element val;

    head = (DListNode*)malloc(sizeof(DListNode));
    init(head);

    dinsert(head, 1);
    dinsert(head, 2);
    dinsert(head, 3);
    print_dlist(head);

    printf("검색 값: ");
    scanf("%d", &val);

    if (search(head, val) != NULL)
        printf("%d is in list\n", val);
    else
        printf("%d isn't in list\n", val);

    return 0;
}