#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_STACK_SIZE 100

typedef int element;

typedef struct {
    element data[MAX_STACK_SIZE];
    int top;
} StackType;

void init_stack(StackType* s)
{
    s->top = -1;
}

int is_empty(StackType* s)
{
    return (s->top == -1);
}

int is_full(StackType* s)
{
    return (s->top == (MAX_STACK_SIZE-1));
}

void push(StackType* s, element item)
{
    if (is_full(s))
    {
        printf("overflow\n");
        exit(1);
    }
    else 
        s->data[++(s->top)] = item;
}

element pop(StackType* s)
{
    if (is_empty(s))
    {
        printf("underflow\n");
        exit(1);
    }
    else
        return s->data[(s->top)--];
}

element peek(StackType* s)
{
    if (is_empty(s))
    {
        printf("underflow\n");
        exit(1);
    }
    else
        return s->data[s->top];
}

int size(StackType* s)
{
    return s->top + 1;
}

int main()
{
    char line[128];
    int length;
    StackType s;

    printf("input string!\n");
    fgets(line, 128, stdin);

    length = strlen(line);
    line[--length] = '\0';
    
    for (int i = 0; i < length/2; ++i)
        push(&s, line[i]);
    
    for (int i = length / 2 + ((length % 2) ? 1 : 0); i < length; ++i)
    {
        if (pop(&s) != line[i])
        {
            printf("No!\n");
            return -1;
        }
    }

    printf("Yes!\n");

    return 0;
}