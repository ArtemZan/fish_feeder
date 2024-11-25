#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "sntp.h"
#include "gpio.h"

static const char *TAG = "ff_wifi";

static int s_retry_num = 0;

EventGroupHandle_t wifi_event_group;

const int MAXIMUM_WIFI_RETRY = 100000;

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

void wifi_init_if_needed()
{
    if (!wifi_event_group)
    {
        wifi_event_group = xEventGroupCreate();
    }
}

void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    printf("Wifi event!: %d\n", (int)event_id);
    if (event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        // gpio_turn_wifi_indicator(false);

        if (s_retry_num < MAXIMUM_WIFI_RETRY)
        {
            vTaskDelay(10 * 1000 / portTICK_PERIOD_MS);
            esp_wifi_connect();
            s_retry_num++;
        }
        else
        {
            xEventGroupSetBits(wifi_event_group, WIFI_FAIL_BIT);
            s_retry_num = 0;
        }
    }
}

void ip_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    printf("IP event!: %d\n", (int)event_id);

    if (event_id == IP_EVENT_STA_GOT_IP)
    {
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
        obtain_time();
        gpio_turn_wifi_indicator(true);
    }
}

int wifi_init_sta()
{
    esp_err_t res = 0;

    ESP_ERROR_CHECK(esp_netif_init());

    esp_netif_create_default_wifi_sta();

    ESP_LOGI(TAG, "wifi_init_sta finished.");

    return res;
}

void ff_wifi_connect()
{
    wifi_init_sta();

    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(esp_wifi_init(&config));

    wifi_init_if_needed();

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, &ip_event_handler, NULL);

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    wifi_config_t sta_config = {
        .sta = {
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false},
        },
    };
    strcpy((char *)sta_config.sta.ssid, "Artem");
    strcpy((char *)sta_config.sta.password, "12345678");

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config));

    esp_wifi_start();
}