#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
// #include "esp_bt.h"

// #include "dev_config_data.h"
// #include "state_mgr.h"

#include "cJSON.h"
#include "esp_mac.h"

#include "nvs.h"

const char *TAG = "nvs";

static nvs_handle_t nvs_cfg_handle;

esp_err_t nvs_init()
{
    esp_err_t err;

    err = nvs_flash_init();

    ESP_ERROR_CHECK(nvs_open("config", NVS_READWRITE, &nvs_cfg_handle));
    // err = nvs_flash_init_partition("config");
    // if (err != ESP_OK)
    // {
    //     ESP_LOGE(TAG, "Error (%s) init partition!\n", esp_err_to_name(err));
    //     return -1;
    // }
    // err = nvs_open_from_partition("config", "config", NVS_READWRITE, &nvs_cfg_handle);

    return ESP_OK;
}