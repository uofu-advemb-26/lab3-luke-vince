#include <stdio.h>
#include <stdbool.h>
#include "threadHelper.h"


TickType_t timeout = pdMS_TO_TICKS(1000);
SemaphoreHandle_t semA;
SemaphoreHandle_t semB;
SemaphoreHandle_t semC;
TaskHandle_t orphaned_lock_handle;
TaskHandle_t not_orphaned_lock_handle;
int orphanCounter = 0;

volatile bool func1TimedOut;
volatile bool func2TimedOut;
volatile int printedValue;
volatile bool myDelayOn;

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

void orphaned_lock(void * params)
{
    while (1) {
        xSemaphoreTake(semC, timeout);
        orphanCounter++;
        if (orphanCounter % 2) {
            continue;
        }
        printf("Count %d\n", orphanCounter);
        xSemaphoreGive(semC);
    }
}

bool call_orphaned_lock(void) {
    semC = xSemaphoreCreateCounting(1, 1);

    xTaskCreate(orphaned_lock, "OrphanedLock", 256, NULL, 1, &orphaned_lock_handle);
    vTaskDelay(500);
    eTaskState method_state = eTaskGetState(orphaned_lock_handle);
    vTaskSuspend(orphaned_lock_handle);
    vTaskDelete(orphaned_lock_handle);

    bool is_method_blocked = method_state == eBlocked;

    vSemaphoreDelete(semC);

    return is_method_blocked;
}

void not_orphaned_lock(void * params)
{
    while (1) {
        xSemaphoreTake(semC, timeout);
        orphanCounter++;
        xSemaphoreGive(semC);
        if (orphanCounter % 2) {
            continue;
        }
        int printedValue = orphanCounter;
        printf("Count %d\n", orphanCounter);
        myDelayOn = true;
        vTaskDelay(pdMS_TO_TICKS(10));
        myDelayOn = false;
    }
}

bool call_not_orphaned_lock(void) {
    semC = xSemaphoreCreateCounting(1, 1);

    xTaskCreate(not_orphaned_lock, "OrphanedLock", 256, NULL, 1, &not_orphaned_lock_handle);
    vTaskDelay(100);
    eTaskState method_state = eTaskGetState(not_orphaned_lock_handle);
    // printf("got the state of not orphaned lock\n");
    vTaskSuspend(not_orphaned_lock_handle);
    vTaskDelete(not_orphaned_lock_handle);

    bool is_method_blocked = method_state == eBlocked;

    vSemaphoreDelete(semC);

    return is_method_blocked && !myDelayOn;
}

int getPrintedValue() {
    return printedValue;
}

bool getMyDelayOn() {
    return myDelayOn;
}