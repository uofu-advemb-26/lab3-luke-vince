#include <FreeRTOS.h>
#include <semphr.h>

void countAndPrintSide(SemaphoreHandle_t semaphore, int timeoutPeriod, int *counter);

void countAndPrintMain(SemaphoreHandle_t semaphore, int timeoutPeriod, int *counter);