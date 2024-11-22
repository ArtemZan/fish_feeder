// #include "i2c.h"
// #include "common.h"

// // static gpio_num_t i2c_gpio_sda = CONFIG_EXAMPLE_I2C_MASTER_SDA;
// // static gpio_num_t i2c_gpio_scl = CONFIG_EXAMPLE_I2C_MASTER_SCL;

// static i2c_port_t i2c_port = I2C_NUM_0;

// uint8_t i2c_data_buffer[64];

// i2c_master_dev_handle_t dev_handle;
// i2c_master_dev_handle_t flash_i2c_handle;

// i2c_master_bus_handle_t i2c_bus_handle;

// void init_i2c_handle()
// {
//     i2c_master_bus_config_t i2c_bus_config = {
//         .clk_source = I2C_CLK_SRC_DEFAULT,
//         .i2c_port = i2c_port,
//         .scl_io_num = 19,
//         .sda_io_num = 18,
//         .glitch_ignore_cnt = 7,
//         .flags.enable_internal_pullup = true,
//     };


//     ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_bus_config, &i2c_bus_handle));
// }

// void add_device(i2c_master_dev_handle_t* handle, unsigned char address) {
//     i2c_device_config_t dev_cfg = {
//         .dev_addr_length = I2C_ADDR_BIT_LEN_7,
//         .device_address = address,
//         .scl_speed_hz = 10000,
//         // .scl_wait_us = 100
//     };

//     ESP_ERROR_CHECK(i2c_master_bus_add_device(i2c_bus_handle, &dev_cfg, handle));
// }

// void ff_i2c_init()
// {
//     init_i2c_handle();
//     printf("dev_handle: %d\n", (int)dev_handle);
//     add_device(&dev_handle, 0x2C);
//     printf("dev_handle: %d\n", (int)dev_handle);
//     printf("dev_handle->ack_check_disable: %d\n", (int)dev_handle->ack_check_disable);
//     // add_device(&flash_i2c_handle, BTL_FLASH_I2C_ADDR);
// }


// int ff_i2c_write_byte(unsigned char address, unsigned char byte) {
//     unsigned char buffer[] = {
//         address,
//         byte
//     };
//     return i2c_master_transmit(dev_handle, buffer, 2, -1);
// }