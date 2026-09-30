#include "FreeRTOS.h"
#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include "unity_config.h"
#include "pico/cyw43_arch.h"
#include <semphr.h>
#include "thread.h"

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

int main (void)
{
    stdio_init_all();
    hard_assert(cyw43_arch_init() == PICO_OK);
    while (1) {
        sleep_ms(5000); // Give time for TTY to attach.
        printf("Start tests\n");
        UNITY_BEGIN();
        RUN_TEST(test_counter_func);
        sleep_ms(5000);
        UNITY_END();
    }
}
