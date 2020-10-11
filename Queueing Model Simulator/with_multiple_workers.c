#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_QUEUE_SIZE 20
// 여러 명의 은행원이 있을 때를 시뮬레이팅 하기 위해 은행원의 수를 나타내는 상수 정의
#define WORKERS_COUNT 2
// rand 함수를 위한 시드 정의
#define SEED 5

typedef struct {
    int id;
    int arrival_time;
    int service_time;
} element;

typedef struct {
    element data[MAX_QUEUE_SIZE];
    int front, rear;
} QueueType;

void error(char *msg)
{
    printf("%s\n", msg);
    exit(1);
}

void init_queue(QueueType* q)
{
    q->front = q->rear = 0;
}

int is_empty(QueueType* q)
{
    return q->front == q->rear;
}

int is_full(QueueType* q)
{
    return (q->rear + 1) % MAX_QUEUE_SIZE == q->front;
}

void enqueue(QueueType* q, element item)
{
    if (is_full(q))
        error("queue is full");
    q->data[(++q->rear) % MAX_QUEUE_SIZE] = item;
}

element dequeue(QueueType* q)
{
    if (is_empty(q))
        error("queue is empty");
    return q->data[(++q->front) % MAX_QUEUE_SIZE];
}

int main()
{
    int minutes = 60;
    int total_wait = 0;
    int total_customer = 0;
    // 여러 명의 은행원이 있을 수 있기때문에,
    // service_time과 service_customer 변수도 독립적으로 관리해야 한다.
    int service_time[WORKERS_COUNT] = {0,};
    int service_customer[WORKERS_COUNT] = {0,};
    // 은행원은 한 명이지만, 대기열 큐는 하나이다.
    QueueType queue;

    // 시드를 직접 지정
    srand(SEED);
    init_queue(&queue);

    for (int clock = 0; clock < minutes; ++clock)
    {
        printf("current time = %d\n", clock);
        // in
        if (rand() % 10 < 3)
        {
            element customer;
            customer.id = total_customer++;
            customer.arrival_time = clock;
            customer.service_time = rand() % 3 + 1;

            enqueue(&queue, customer);
            printf("customer: %d, arrival time: %d, service time: %d\n",
                    customer.id, customer.arrival_time, customer.service_time);
        }

        for (int i = 0; i < WORKERS_COUNT; ++i)
        {
            // servicing
            if (service_time[i] > 0)
            {
                printf("worker %d - servicing customer: %d\n", i, service_customer[i]);
                --service_time[i];
            }
            // free
            else
            {
                if (is_empty(&queue))
                {
                    printf("worker %d - resting\n", i);
                }
                else
                {
                    element customer = dequeue(&queue);
                    service_customer[i] = customer.id;
                    service_time[i] = customer.service_time;
                    printf("worker %d - customer %d, start time: %d, wait time %d\n",
                           i, customer.id, clock, clock - customer.arrival_time);

                    total_wait += clock - customer.arrival_time;
                }
            }
        }
    }

    printf("total wait time = %d\n", total_wait);

    return 0;
}