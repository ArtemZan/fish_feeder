#include <stdio.h>
#include "driver/gpio.h"
#include "BLDC.h"
#include "freertos/FreeRTOS.h"
#include "nvs.h"
#include "wifi.h"
#include "esp_event.h"
#include "esp_sntp.h"
#include "sntp.h"
#include <math.h>
#include "gpio.h"

BLDC wires[4];

void rotate_one_section()
{
    // 2048 steps per revolution
    for (int step = 0; step < 2048 / 10; step++)
    {
        // 4 steps per cycle
        int currentCycleStep = step % 4;

        for (int cycleStep = 0; cycleStep < 4; cycleStep++)
        {
            BLDC_set_throttle(&wires[cycleStep], cycleStep == currentCycleStep ? 1 : 0);
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    BLDC_set_throttle(&wires[0], 0);
    BLDC_set_throttle(&wires[1], 0);
    BLDC_set_throttle(&wires[2], 0);
    BLDC_set_throttle(&wires[3], 0);
}

void app_main(void)
{
    gpio_init();
    nvs_init();

    esp_event_loop_create_default();

    ff_wifi_connect();

    gpio_turn_wifi_indicator(false);

    sntp_wait_until_obtained_time();

    BLDC_create(&wires[0], 26, LEDC_CHANNEL_0);
    BLDC_create(&wires[1], 25, LEDC_CHANNEL_1);
    BLDC_create(&wires[2], 33, LEDC_CHANNEL_2);
    BLDC_create(&wires[3], 32, LEDC_CHANNEL_3);

    BLDC_set_throttle(&wires[0], 0);
    BLDC_set_throttle(&wires[1], 0);
    BLDC_set_throttle(&wires[2], 0);
    BLDC_set_throttle(&wires[3], 0);

    while (true)
    {
        time_t now;
        time(&now);
        struct tm timeinfo = {0};
        localtime_r(&now, &timeinfo);

        const int currentHours = (int)timeinfo.tm_hour;
        const int currentMinutes = (int)timeinfo.tm_min;

        printf("Time now: %d:%d\n", currentHours, currentMinutes);

        int next_yummy_hour = ((int)((currentHours + 12 - 8) / 12) * 12 + 8) % 24;
        printf("next_yummy_hour: %d\n", next_yummy_hour);

        int minutes_to_wait = (24 + next_yummy_hour - currentHours) % 24 * 60 - currentMinutes;

        printf("Time to wait: %d:%d:00\n", minutes_to_wait / 60, minutes_to_wait % 60);

        vTaskDelay(minutes_to_wait * 60 * 1000 / portTICK_PERIOD_MS);

        // Rotate 2 sections
        rotate_one_section();
        rotate_one_section();

        vTaskDelay(60 * 1000 / portTICK_PERIOD_MS);
    }
}
