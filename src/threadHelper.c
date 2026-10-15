#include <stdio.h>
#include "threadHelper.h"


TickType_t timeout = pdMS_TO_TICKS(1000);

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

// Return true if deadlock occured.
bool deadlock() {
    SemaphoreHandle_t semA = xSemaphoreCreateCounting(1, 1);
    SemaphoreHandle_t semB = xSemaphoreCreateCounting(1, 1);

    bool deadlockIn1 = func1(semA, semB);
    bool deadlockIn2 = func2(semA, semB);

    return !deadlockIn1 || !deadlockIn2;
}

bool func1(SemaphoreHandle_t semA, SemaphoreHandle_t semB) {
    // Take A
    bool tookA = xSemaphoreTake(semA, timeout) == pdTRUE;
    // Wait
    vTaskDelay(500);
    // Take B
    bool tookB = xSemaphoreTake(semB, timeout) == pdTRUE;

    // Deadlock just happend and timeout happened.
    return tookB
}

bool func2(SemaphoreHandle_t semA, SemaphoreHandle_t semB) {
    // Wait Less
    vTaskDelay(250);
    // Take B
    bool tookB = xSemaphoreTake(semB, timeout) == pdTRUE;
    // Take A
    bool tookA = xSemaphoreTake(semA, timeout) == pdTRUE;

    // Deadlock just happend and timeout happened.
    return tookA
}

