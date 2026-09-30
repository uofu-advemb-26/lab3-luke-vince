#ifndef _THREAD_H_
#define _THREAD_H_

#include "FreeRTOS.h"
#include "task.h"
#include "pico/cyw43_arch.h"

int counterFunc(char *callSrc, SemaphoreHandle_t semaphore, int *counter, int timeoutPeriod);

#endif