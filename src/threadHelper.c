#include <stdio.h>
#include <stdbool.h>
#include "threadHelper.h"


TickType_t timeout = pdMS_TO_TICKS(1000);
SemaphoreHandle_t semA;
SemaphoreHandle_t semB;

volatile bool func1TimedOut;
volatile bool func2TimedOut;

void countAndPrintSide(SemaphoreHandle_t semaphore, int timeoutPeriod, int *counter) {
    xSemaphoreTake(semaphore, timeoutPeriod);
    {
        (*counter) += 1;
        printf("hello world from %s! Count %d\n", "thread", *counter);
    }
    xSemaphoreGive(semaphore);
}

void countAndPrintMain(SemaphoreHandle_t semaphore, int timeoutPeriod, int *counter) {
    xSemaphoreTake(semaphore, timeoutPeriod);
    {
        (*counter)++;
        printf("hello world from %s! Count %d\n", "main", *counter);
    }
    xSemaphoreGive(semaphore);
}

void func1(void *params) {
    // Take A
    bool tookA = xSemaphoreTake(semA, timeout) == pdTRUE;
    // Wait
    vTaskDelay(500);
    // Take B
    bool tookB = xSemaphoreTake(semB, timeout) == pdTRUE;

    func1TimedOut = !tookB;

    if(tookB)
        xSemaphoreGive(semB);
    if(tookA)
        xSemaphoreGive(semA);

    // Deadlock just happend and timeout happened.
    vTaskDelete(NULL);
}

void func2(void * params) {
    // Wait Less
    vTaskDelay(250);
    // Take B
    bool tookB = xSemaphoreTake(semB, timeout) == pdTRUE;
    // Take A
    bool tookA = xSemaphoreTake(semA, timeout) == pdTRUE;

    func2TimedOut = !tookA;

    if(tookA)
        xSemaphoreGive(semA);
    if(tookB)
        xSemaphoreGive(semB);

    // Deadlock just happend and timeout happened.
    vTaskDelete(NULL);
}

// Return true if deadlock occured.
void deadlock(void) {
    semA = xSemaphoreCreateCounting(1, 1);
    semB = xSemaphoreCreateCounting(1, 1);
    func1TimedOut = false;
    func2TimedOut = false;

    xTaskCreate(func1, "Func1", 256, NULL, 1, NULL);
    xTaskCreate(func2, "Func2", 256, NULL, 1, NULL);
}

bool deadlockOccured(void) {
    return func1TimedOut || func2TimedOut;
}

void cleanupDeadlock(void) {
    vSemaphoreDelete(semA);
    vSemaphoreDelete(semB);
}

