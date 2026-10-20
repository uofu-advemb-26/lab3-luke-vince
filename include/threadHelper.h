#include <FreeRTOS.h>
#include <semphr.h>

void countAndPrintSide(SemaphoreHandle_t semaphore, int timeoutPeriod, int *counter);

void countAndPrintMain(SemaphoreHandle_t semaphore, int timeoutPeriod, int *counter);

void deadlock(void);

bool deadlockOccured(void);

void cleanupDeadlock();

bool call_orphaned_lock(void);