#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "esp_sleep.h"
#include "nvs_flash.h"
#include "esp_netif_sntp.h"
#include "lwip/ip_addr.h"
#include "esp_sntp.h"

static const char *TAG = "SNTP";

#define CONFIG_SNTP_TIME_SERVER "pool.ntp.org"

void obtain_time(void)
{
    ESP_LOGI(TAG, "Initializing and starting SNTP");

    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG(CONFIG_SNTP_TIME_SERVER);
    // config.sync_cb = time_sync_notification_cb;     // Note: This is only needed if we want

    esp_netif_sntp_init(&config);

    // print_servers();

    // wait for time to be set
    time_t now = 0;
    struct tm timeinfo = {0};
    int retry = 0;
    const int retry_count = 15;
    while (esp_netif_sntp_sync_wait(2000 / portTICK_PERIOD_MS) == ESP_ERR_TIMEOUT && ++retry < retry_count)
    {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
    }
    printf("Set time: %d\n", (int)now);
    time(&now);
    printf("Set time: %d\n", (int)now);
    localtime_r(&now, &timeinfo);
    printf("Set time: %d\n", (int)now);

    // ESP_ERROR_CHECK( example_disconnect() );
    esp_netif_sntp_deinit();
}