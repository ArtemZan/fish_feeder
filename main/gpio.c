#include "gpio.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"


const int WIFI_INDICATOR_GPIO = 18;
static const char* TAG = "ff_gpio";

void gpio_init() 
{
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = 1ULL << WIFI_INDICATOR_GPIO;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);
}

void gpio_turn_wifi_indicator(bool on) {
    ESP_LOGI(TAG, "gpio_turn_wifi_indicator: %d", (int)on);
    gpio_set_level(WIFI_INDICATOR_GPIO, (int)on);
}