#include <stdio.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/cyw43_arch.h>
#include "thread.h"

#define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

#define SIDE_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define SIDE_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

SemaphoreHandle_t semaphore;

int counter;
int on;

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

// Adding 1 to counter and printing something every tenth of a second.
void side_thread(void *params)
{
	while (1) {
        vTaskDelay(100);
        // counter += 1;
		// printf("hello world from %s! Count %d\n", "thread", counter);
        counterFunc("thread", semaphore, &counter, 500);
	}
}

// Turning an LED on or off and incrementing counter every tenth of a second.
void main_thread(void *params)
{
	while (1) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, on);
        vTaskDelay(100);
		// printf("hello world from %s! Count %d\n", "main", counter++);
        counterFunc("main", semaphore, &counter, 500);
        on = !on;
	}
}

int main(void)
{
    stdio_init_all();
    hard_assert(cyw43_arch_init() == PICO_OK);
    on = false;
    counter = 0;
    TaskHandle_t main, side;
    semaphore = xSemaphoreCreateCounting(1, 1);
    xTaskCreate(main_thread, "MainThread",
                MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, &main);
    xTaskCreate(side_thread, "SideThread",
                SIDE_TASK_STACK_SIZE, NULL, SIDE_TASK_PRIORITY, &side);
    vTaskStartScheduler();
	return 0;
}


/*PREDICTION: Since the semaphore isn't currently being used at all, it is likely that
the counter value is going to be incrementing unpredictably. As a result, main and
side-thread might not necessarily increment in consistent ways as they print their
values out. In other words, there is going to be a race condition between the two threads.*/

/*REALITY: Exactly that has happened; we saw the output mostly alternate, but every so
often one thread might run twice compared to the other, leading to the incrementing pattern
being thrown off*/