#include <stdio.h>

void printHanoi(int n, char left, char mid, char right)
{
    if (n == 1)
    {
        printf("원판 %d(을)를 %c에서 %c로(으로) 옮깁니다.\n", n, left, right);
    }
    else
    {
        printHanoi(n - 1, left, right, mid);
        printf("원판 %d(을)를 %c에서 %c로(으로) 옮깁니다.\n", n, left, right);
        printHanoi(n - 1, mid, left, right);
    }
}

int main()
{
    int k;
    scanf("%d", &k);
    printHanoi(k, 'A', 'B', 'C');
    return 0;
}