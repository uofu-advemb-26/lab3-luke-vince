#ifndef _THREAD_H_
#define _THREAD_H_

#include <FreeRTOS.h>
#include <task.h>
#include <pico/cyw43_arch.h>
#include <semphr.h>

struct deadlockArgs {
    SemaphoreHandle_t a;
    SemaphoreHandle_t b;
    int counter;
    int id;
};

struct test5Data {
    SemaphoreHandle_t semaphore;
    TickType_t timeout;
    int counter;
};

void deadlockedFunct(void *args);
int orphaned_lock(void *args);
int unorphaned_lock(void *args);

int counterFunc(char *callSrc, SemaphoreHandle_t semaphore, int *counter, int timeoutPeriod);

#endif