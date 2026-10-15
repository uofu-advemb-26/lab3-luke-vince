#include <stdio.h>
#include "threadHelper.h"

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