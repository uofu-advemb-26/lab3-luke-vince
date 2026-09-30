#include "threadhelpers.h"

int timeoutPeriod = 1000000000;

int counterFunc(char *callSrc, SemaphoreHandle_t semaphore, int *counter, int timeoutPeriod) {
    if(xSemaphoreTake(semaphore, timeoutPeriod) == 0) {
        return pdFAIL;      //indicate that the function call has failed because the semaphore isn't available
    }

    //if xSemaphoreTake didn't return a 0 then we have successfully acquired the semaphore
    else {
        (*counter)++;
        printf("hello world from %s! Count %d\n", callSrc, *counter);
    }
    xSemaphoreGive(semaphore);      //release the semaphore
    return pdTRUE;
}

int deadlock() {
    SemaphoreHandle_t semA = xSemaphoreCreateCounting(1, 1);
    SemaphoreHandle_t semB = xSemaphoreCreateCounting(1, 1);

    func1(semA, semB);
    func2(semA, semB);
}

int func1(SemaphoreHandle_t semA, SemaphoreHandle_t semB) {
    // Take lock A
    xSemaphoreTake(semA, timeoutPeriod);
    
    // Delay
    vTaskDelay(500);
    // Take lock B
    xSemaphoreTake(semB, timeoutPeriod);
    // Nothing past here will ever happen
}


int func2(SemaphoreHandle_t semA, SemaphoreHandle_t semB) {
    // Delay
    vTaskDelay(250);
    // Take lock B
    xSemaphoreTake(semB, timeoutPeriod);
    // Take lock A
    xSemaphoreTake(semA, timeoutPeriod);
    // Nothing past here will ever happen
}
