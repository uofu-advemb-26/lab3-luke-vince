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
    bool deadlockOccured = deadlock();
    TEST_ASSERT_TRUE_MESSAGE(deadlockOccured, "Deadlock didn't occur I guess.");
}

int main (void)
{
    stdio_init_all();
    while (1) {
        sleep_ms(5000); // Give time for TTY to attach.
        printf("Start tests\n");
        UNITY_BEGIN();
        RUN_TEST(test_variable_assignment);
        RUN_TEST(test_multiplication);
        RUN_TEST(test_side_count);
        RUN_TEST(test_main_count);
        sleep_ms(5000);
        UNITY_END();
    }
}
