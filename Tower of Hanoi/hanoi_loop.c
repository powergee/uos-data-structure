#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct
{
    int *disks;
    int count;
} Pole;

Pole* createPole(int capacity)
{
    Pole* p = (Pole*)malloc(sizeof(Pole));
    p->count = 0;
    p->disks = (int*)malloc(sizeof(int) * capacity);
    return p;
}

void destroyPole(Pole* p)
{
    free(p->disks);
    free(p);
}

int removeTop(Pole* p)
{
    int top;
    if (p->count <= 0)
        return INT_MAX;
    top = p->disks[p->count-1];
    --(p->count);
    return top;
}

int getTop(Pole* p)
{
    if (p->count <= 0)
        return INT_MAX;
    return p->disks[p->count-1];
}

void putDisk(Pole* p, int dSize)
{
    p->disks[p->count++] = dSize;
}

void makeValidMovement(Pole* s1, Pole* s2, int name1, int name2)
{
    if (s1->count == 0 && s2->count == 0)
        return;
    
    if (s1->count == 0 || getTop(s1) > getTop(s2))
    {
        putDisk(s1, removeTop(s2));
        printf("%d %d\n", name2, name1);
    }
    else
    {
        putDisk(s2, removeTop(s1));
        printf("%d %d\n", name1, name2);
    }
}

void swapValue(int* v1, int* v2)
{
    int temp = *v1;
    *v1 = *v2;
    *v2 = temp;
}

void printHanoi(int n, int start, int free, int dest)
{
    int count = (1 << n) - 1;
    Pole* startPole = createPole(n);
    Pole* freePole = createPole(n);
    Pole* destPole = createPole(n);

    printf("%d\n", count);

    if (n % 2 == 0)
        swapValue(&free, &dest);
    
    for (int s = n; s > 0; --s)
        putDisk(startPole, s);

    for (int i = 1; i <= count; ++i)
    {
        if (i % 3 == 1)
            makeValidMovement(startPole, destPole, start, dest);
        if (i % 3 == 2)
            makeValidMovement(startPole, freePole, start, free);
        if (i % 3 == 0)
            makeValidMovement(freePole, destPole, free, dest);
    }

    destroyPole(startPole);
    destroyPole(freePole);
    destroyPole(destPole);
}

int main()
{
    int k;
    scanf("%d", &k);
    printHanoi(k, 1, 2, 3);

    return 0;
}