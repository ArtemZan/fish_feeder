#include <stdio.h>
#include "driver/gpio.h"
#include "BLDC.h"
#include "freertos/FreeRTOS.h"
#include "nvs.h"
#include "wifi.h"
#include "esp_event.h"

BLDC wires[4];

void app_main(void)
{
    nvs_init();

    esp_event_loop_create_default();

    ff_wifi_connect();
    while (true)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    return;
    BLDC_create(&wires[0], 26, LEDC_CHANNEL_0);
    BLDC_create(&wires[1], 25, LEDC_CHANNEL_1);
    BLDC_create(&wires[2], 33, LEDC_CHANNEL_2);
    BLDC_create(&wires[3], 32, LEDC_CHANNEL_3);


    int c = 0;
    while (true)
    {

        for (int i = 0; i < 4; i++)
        {
            BLDC_set_throttle(&wires[i], c == i ? 1 : 0);
        }

        c++;

        if (c == 4)
        {
            c = 0;
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
