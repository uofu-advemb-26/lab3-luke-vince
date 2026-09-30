#include "threadhelpers.h"

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
