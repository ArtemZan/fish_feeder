#include "driver/i2c_master.h"
#include "/Users/admin/esp/esp-idf/components/esp_driver_i2c/i2c_private.h"

extern i2c_master_dev_handle_t dev_handle;
extern i2c_master_dev_handle_t flash_i2c_handle;

extern uint8_t i2c_data_buffer[64];

void ff_i2c_init();

int ff_i2c_write_byte(unsigned char address, unsigned char byte);