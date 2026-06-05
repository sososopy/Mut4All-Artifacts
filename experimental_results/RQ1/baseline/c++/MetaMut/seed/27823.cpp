
#include <stdio.h>
#include <pthread.h>

volatile int DONE = 0;
volatile int que[65536];
volatile unsigned short head = 0;
volatile unsigned short tail = 0;

void in(int data)
{
        while ((tail - head == 1) || (head - tail == 65535));
        que[head++] = data;
//      que[head] = data;
//      head++;
}

int out(void)
{
        while (head == tail);
        return que[tail++];
}

void* test0(void* param)
{
        for (int c = 0; c < 10000000; c++) in(c);
        putchar('0'); DONE++;
}

void* test1(void* param)
{
        for (int c = 0; c < 10000000; c++) if (out() != c) putchar('E');
        putchar('1'); DONE++;
}

int main(void)
{
        pthread_t thread1;
        pthread_t thread2;

        pthread_create(&thread1, NULL, test0, (void*)0);
        pthread_create(&thread2, NULL, test1, (void*)0);
        while (DONE < 2);
}
