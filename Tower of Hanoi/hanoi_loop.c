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

void makeValidMovement(Pole* s1, Pole* s2, char name1, char name2)
{
    if (s1->count == 0 && s2->count == 0)
        return;
    
    if (s1->count == 0 || getTop(s1) > getTop(s2))
    {
        int disk = removeTop(s2);
        putDisk(s1, disk);
        printf("원판 %d(을)를 %c에서 %c로(으로) 옮깁니다.\n", disk, name2, name1);
    }
    else
    {
        int disk = removeTop(s1);
        putDisk(s2, disk);
        printf("원판 %d(을)를 %c에서 %c로(으로) 옮깁니다.\n", disk, name1, name2);
    }
}

void swapValue(char* v1, char* v2)
{
    char temp = *v1;
    *v1 = *v2;
    *v2 = temp;
}

void printHanoi(int n, char start, char free, char dest)
{
    Pole* startPole = createPole(n);
    Pole* freePole = createPole(n);
    Pole* destPole = createPole(n);

    for (int s = n; s > 0; --s)
        putDisk(startPole, s);

    if (n % 2)
    {
        for (int i = 0; destPole->count < n; ++i)
        {
            if (i % 3 == 0)
                makeValidMovement(startPole, destPole, start, dest);
            if (i % 3 == 1)
                makeValidMovement(startPole, freePole, start, free);
            if (i % 3 == 2)
                makeValidMovement(freePole, destPole, free, dest);
        }
    }
    else
    {
        for (int i = 0; destPole->count < n; ++i)
        {
            if (i % 3 == 0)
                makeValidMovement(startPole, freePole, start, free);
            if (i % 3 == 1)
                makeValidMovement(startPole, destPole, start, dest);
            if (i % 3 == 2)
                makeValidMovement(freePole, destPole, free, dest);
        }
    }

    destroyPole(startPole);
    destroyPole(freePole);
    destroyPole(destPole);
}

int main()
{
    int k;
    scanf("%d", &k);
    printHanoi(k, 'A', 'B', 'C');

    return 0;
}