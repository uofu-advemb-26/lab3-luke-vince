#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include "unity_config.h"
#include "threadHelper.h"

void setUp(void) {}

void tearDown(void) {}

void test_variable_assignment()
{
    int x = 1;
    TEST_ASSERT_TRUE_MESSAGE(x == 1,"Variable assignment failed.");
}

void test_multiplication(void)
{
    int x = 30;
    int y = 6;
    int z = x / y;
    TEST_ASSERT_TRUE_MESSAGE(z == 5, "Multiplication of two integers returned incorrect value.");
}

void test_side_count()
{
    SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1, 1);
    int counter = 0;
    TickType_t timeoutPeriod = portMAX_DELAY;

    countAndPrintSide(semaphore, timeoutPeriod, &counter);
    TEST_ASSERT_TRUE_MESSAGE(counter == 1, "Count is not chill");
}

void test_main_count()
{
    SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1, 1);
    int counter = 0;
    TickType_t timeoutPeriod = portMAX_DELAY;

    countAndPrintMain(semaphore, timeoutPeriod, &counter);
    TEST_ASSERT_TRUE_MESSAGE(counter == 1, "Count is not chill");
}

void deadlock_test()
{
    printf("Beginning of Deadlock Test\n");
    deadlock();
    printf("After method before delay.\n");
    // Wait for the functions to timeout.
    vTaskDelay(200);
    printf("After 200ms delay\n");
    vTaskDelay(200);
    printf("After 400ms delay\n");
    vTaskDelay(200);
    printf("After 600ms delay\n");
    vTaskDelay(1400);
    
    printf("After method after delay.\n");

    TEST_ASSERT_TRUE_MESSAGE(deadlockOccured(), "Neither task timed out while waiting for the other semaphore.");
    printf("After assert statement\n");
    cleanupDeadlock();
    printf("After call to cleanup\n");
}

void run_tests(void * params) {
    while (1) {
        sleep_ms(15000); // Give time for TTY to attach.
        printf("Start tests\n");
        UNITY_BEGIN();
        RUN_TEST(test_variable_assignment);
        RUN_TEST(test_multiplication);
        RUN_TEST(test_side_count);
        RUN_TEST(test_main_count);
        RUN_TEST(deadlock_test);
        sleep_ms(5000);
        UNITY_END();
    }
}

int main (void)
{
    stdio_init_all();
    // Create the task
    xTaskCreate(run_tests, "RunTests", configMINIMAL_STACK_SIZE * 4, NULL, 1, NULL);
    // Start the scheduler
    vTaskStartScheduler();

    printf("This should never happen.  This is after vTaskStartscheduler().");
    
}
