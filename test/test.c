#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include <pico/cyw43_arch.h>
#include <semphr.h>
#include "threadhelpers.h"

#define TEST_RUNNER_PRIORITY ( tskIDLE_PRIORITY + 5UL )

SemaphoreHandle_t semA;
SemaphoreHandle_t semB;

void setUp(void) {}

void tearDown(void) {}

// void test_variable_assignment()
// {
//     int x = 1;
//     TEST_ASSERT_TRUE_MESSAGE(x == 1,"Variable assignment failed.");
// }

// void test_multiplication(void)
// {
//     int x = 30;
//     int y = 6;
//     int z = x / y;
//     TEST_ASSERT_TRUE_MESSAGE(z == 5, "Multiplication of two integers returned incorrect value.");
// }

void test_counter_func() {
    int counter = 0;
    SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1, 1);

    int returnStatus = counterFunc("main", semaphore, &counter, 500);

    TEST_ASSERT_EQUAL_MESSAGE(1, returnStatus, "ReturnStatus returned a failed counter function operation call");
    TEST_ASSERT_EQUAL_MESSAGE(1, counter, "Counter incorrectly incremented by the counter function");
}

void test_deadlocked_code() {

    #define TASK1_PRIORITY      ( tskIDLE_PRIORITY - 1UL )
    #define TASK1_STACK_SIZE configMINIMAL_STACK_SIZE

    #define TASK2_PRIORITY      ( tskIDLE_PRIORITY - 1UL )
    #define TASK2_STACK_SIZE configMINIMAL_STACK_SIZE

    TaskHandle_t task1, task2;

    semA = xSemaphoreCreateCounting(1, 1);
    semB = xSemaphoreCreateCounting(1, 1);

    struct deadlockArgs task1Args = {semA, semB, 5, 1};
    struct deadlockArgs task2Args = {semB, semA, 20, 2};
    

    xTaskCreate(deadlockedFunct, "task1",
                TASK1_STACK_SIZE, (void*)&task1Args, TASK1_PRIORITY, &task1);
    xTaskCreate(deadlockedFunct, "task2",
                TASK2_STACK_SIZE, (void*)&task2Args, TASK2_PRIORITY, &task2);

    printf("Threads made. Waiting 5000 ticks.\n");
    vTaskDelay(5000);
    printf("Delay complete.\n");

    TEST_ASSERT_EQUAL_INT(0, uxSemaphoreGetCount(semA));
    TEST_ASSERT_EQUAL_INT(0, uxSemaphoreGetCount(semB));

    TEST_ASSERT_EQUAL_MESSAGE(7, task1Args.counter, "Task 1 should've only incremented the counter twice!\n");
    TEST_ASSERT_EQUAL_MESSAGE(22, task2Args.counter, "Task 2 should've only incremented the counter twice!\n");

    printf("\tCleaning up hung threads...\n");
    vTaskDelete(task1);
    vTaskDelete(task2);
}

void test_orphaned_lock() {
    printf("Testing orphaned lock code\n");
    semA = xSemaphoreCreateCounting(1, 1);
    struct test5Data orphanedLockData = {semA, 500, 1};

    int result = orphaned_lock((void*)&orphanedLockData);
    TEST_ASSERT_EQUAL_MESSAGE(2, orphanedLockData.counter, "This should be 2");
    TEST_ASSERT_EQUAL(pdTRUE, result);
    TEST_ASSERT_EQUAL_INT(1, uxSemaphoreGetCount(semA));

    /**
     * Because we are now going to be seeing "3" on the counter, the 
     * function should return 0 and NOT release the lock 
     */
    result = orphaned_lock((void*)&orphanedLockData);
    TEST_ASSERT_EQUAL_MESSAGE(3, orphanedLockData.counter, "This should be 3");
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL_INT(0, uxSemaphoreGetCount(semA));

    /**
     * We shouldn't see any incrementing now
     */
    result = orphaned_lock((void*)&orphanedLockData);
    TEST_ASSERT_EQUAL_MESSAGE(3, orphanedLockData.counter, "This should be 3");
    TEST_ASSERT_EQUAL(pdFALSE, result);
    TEST_ASSERT_EQUAL_INT(0, uxSemaphoreGetCount(semA));
}

void test_unorphaned_lock() {
printf("Testing orphaned lock code\n");
    semA = xSemaphoreCreateCounting(1, 1);
    struct test5Data unorphanedLockData = {semA, 500, 1};

    int result = unorphaned_lock((void*)&unorphanedLockData);
    TEST_ASSERT_EQUAL_MESSAGE(2, unorphanedLockData.counter, "This should be 2");
    TEST_ASSERT_EQUAL(pdTRUE, result);
    TEST_ASSERT_EQUAL_INT(1, uxSemaphoreGetCount(semA));

    /**
     * Because we are now going to be seeing "3" on the counter.
     */
    result = unorphaned_lock((void*)&unorphanedLockData);
    TEST_ASSERT_EQUAL_MESSAGE(3, unorphanedLockData.counter, "This should be 3");
    TEST_ASSERT_EQUAL(pdTRUE, result);
    TEST_ASSERT_EQUAL_INT(1, uxSemaphoreGetCount(semA));

    /**Because we can now increment to 4, we should see "count" be printed again too */
    result = unorphaned_lock((void*)&unorphanedLockData);
    TEST_ASSERT_EQUAL_MESSAGE(4, unorphanedLockData.counter, "This should be 4");
    TEST_ASSERT_EQUAL(pdTRUE, result);
    TEST_ASSERT_EQUAL_INT(1, uxSemaphoreGetCount(semA));
}

/**
 * We have to split off the test runner into its own task so that it gets a priority assignment that we have control over
 */
void test_runner(void) {
    while (1) {
        sleep_ms(5000); // Give time for TTY to attach.
        printf("Start tests\n");
        UNITY_BEGIN();
        RUN_TEST(test_counter_func);
        RUN_TEST(test_deadlocked_code);
        RUN_TEST(test_orphaned_lock);
        RUN_TEST(test_unorphaned_lock);
        sleep_ms(5000);
        UNITY_END();
    }
}

int main (void)
{
    stdio_init_all();
    hard_assert(cyw43_arch_init() == PICO_OK);
    xTaskCreate(test_runner, "Test Thread", configMINIMAL_STACK_SIZE, NULL, TEST_RUNNER_PRIORITY, NULL);
    vTaskStartScheduler();
    return 0;
}
