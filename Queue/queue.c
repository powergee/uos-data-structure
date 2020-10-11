#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 20

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
    int service_time = 0;
    int service_customer;
    QueueType queue;

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

        // servicing
        if (service_time > 0)
        {
            printf("servicing customer: %d\n", service_customer);
            --service_time;
        }
        // free
        else
        {
            if (!is_empty(&queue))
            {
                element customer = dequeue(&queue);
                service_customer = customer.id;
                service_time = customer.service_time;
                printf("customer %d, start time: %d, wait time %d\n",
                        customer.id, clock, clock - customer.arrival_time);
                
                total_wait += clock - customer.arrival_time;
            }
        }
    }

    printf("total wait time = %d\n", total_wait);

    return 0;
}