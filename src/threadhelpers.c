#include "threadhelpers.h"

int timeoutPeriod = 1000000000;

int counterFunc(char *callSrc, SemaphoreHandle_t semaphore, int *counter, int timeoutPeriod)
{
    if (xSemaphoreTake(semaphore, timeoutPeriod) == 0)
    {
        return pdFAIL; // indicate that the function call has failed because the semaphore isn't available
    }

    // if xSemaphoreTake didn't return a 0 then we have successfully acquired the semaphore
    else
    {
        (*counter)++;
        printf("hello world from %s! Count %d\n", callSrc, *counter);
    }
    xSemaphoreGive(semaphore); // release the semaphore
    return pdTRUE;
}

void deadlockedFunct(void *args)
{
    struct deadlockArgs *dargs = (struct deadlockArgs *)args;

    printf("Thread %d initialized. Incrementing counter.\n", dargs->id);
    dargs->counter++;
    xSemaphoreTake(dargs->a, portMAX_DELAY);
    {
        dargs->counter++;
        printf("\tThread %d now inside lock %d. Incrementing again.\n", dargs->id, dargs->a);
        vTaskDelay(100);
        xSemaphoreTake(dargs->b, portMAX_DELAY);
        {
            printf("\tThread %d now inside lock %d. Incrementing again.\n", dargs->id, dargs->b);
            dargs->counter++;
        }
        xSemaphoreGive(dargs->b);
        printf("Semaphore %d released.\n", dargs->b);
    }
    xSemaphoreGive(dargs->a);
    printf("Semaphore %d released.\n", dargs->a);
    vTaskSuspend(NULL);
}

int orphaned_lock(void *args)
{
    struct test5Data *testArgs = (struct test5Data *)args;
    if (xSemaphoreTake(testArgs->semaphore, testArgs->timeout) == pdFALSE) {
        return pdFALSE;
    }
    
    testArgs->counter++;
    //this is checking divisibility by 2 but plugging the 
    //result of % 2 directly into the if statement, so 2 % 2 = 0
    //means that we will NOT return 0
    if (testArgs->counter % 2)
    {
        return 0;
    }
    printf("Count %d\n", testArgs->counter);

    xSemaphoreGive(testArgs->semaphore);
    return pdTRUE;
}

int unorphaned_lock(void*args) {
    struct test5Data *testArgs = (struct test5Data *)args;
    if (xSemaphoreTake(testArgs->semaphore, testArgs->timeout) == pdFALSE) {
        return pdFALSE;
    }
    
    testArgs->counter++;
    //if 2 % 2 = 0 then we want to continue and print count
    if (!(testArgs->counter % 2))
    {
        printf("Count %d\n", testArgs->counter);
    }
    
    xSemaphoreGive(testArgs->semaphore);
    return pdTRUE;
}