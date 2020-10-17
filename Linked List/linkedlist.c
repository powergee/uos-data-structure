#include <stdio.h>
#include <stdlib.h>

typedef int element;
typedef struct _ListNode {
    element data;
    struct _ListNode* link;
} ListNode;

void error(char* message)
{
    printf("%s\n", message);
    exit(1);
}

ListNode* insert_first(ListNode* head, element value)
{
    ListNode* p = (ListNode*)malloc(sizeof(ListNode));

    p->data = value;
    p->link = head;
    head = p;

    return head;
}

ListNode* insert(ListNode* head, ListNode* pre, element value)
{
    ListNode* p = (ListNode*)malloc(sizeof(ListNode));

    p->data = value;
    p->link = pre->link;
    pre->link = p;

    return head;
}

ListNode* insert_sorted(ListNode* head, element value)
{
    ListNode* p = (ListNode*)malloc(sizeof(ListNode));
    ListNode* cur;

    p->data = value;
    p->link = NULL;

    if (head == NULL)
        head = p;
    else
    {
        cur = head;
        while (cur->link != NULL && cur->link->data < p->data)
            cur = cur->link;

        p->link = cur->link;
        cur->link = p;
    }

    return head;
}

ListNode* delete_first(ListNode* head)
{
    ListNode* toRemove;

    toRemove = head;
    head = toRemove->link;
    free(toRemove);

    return head;
}

ListNode* delete(ListNode* head, ListNode* pre)
{
    ListNode* toRemove;

    toRemove = pre->link;
    pre->link = toRemove->link;
    free(toRemove);

    return head;
}

void print_list(ListNode* head)
{
    for (ListNode* p = head; p != NULL; p = p->link)
        printf("%d->", p->data);
    printf("NULL\n");
}

int main()
{
    ListNode* head = NULL;
    
    for (int i = 0; i < 5; ++i)
    {
        head = insert_sorted(head, i);
        print_list(head);
    }
    
    head = insert_sorted(head, 8);
    print_list(head);
    head = insert_sorted(head, 7);
    print_list(head);
    head = insert_sorted(head, 6);
    print_list(head);

    for (int i = 0; i < 5; ++i)
    {
        head = delete_first(head);
        print_list(head);
    }

    return 0;
}