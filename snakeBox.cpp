// Sweep through all 7-bit I2C addresses, to see if any slaves are present on
// the I2C bus. Print out a table that looks like this:
//
// I2C Bus Scan
//   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
// 0
// 1       @
// 2
// 3             @
// 4
// 5
// 6
// 7
//
// E.g. if slave addresses 0x12 and 0x34 were acknowledged.

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// I2C reserves some addresses for special purposes. We exclude these from the scan.
// These are any addresses of the form 000 0xxx or 111 1xxx
// bool reserved_addr(uint8_t addr) {
//     return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
// }

int main() {
    // Turn on the LED!
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);

    // Enable UART so we can print status output
    stdio_init_all();

    // Setup I2C
    i2c_init(i2c_default, 40 * 1000);   //TODO:DS: can this go back to 100? idk bruh
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    // gpio_disable_pulls(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    // gpio_disable_pulls(PICO_DEFAULT_I2C_SCL_PIN);
    
    // Enter the main loop
    while(true) {
        int ret;
        uint8_t txdata[1];
        txdata[0] = 1;
        uint8_t rxdata[3];
        printf("Writing!\n");
        ret = i2c_write_blocking(i2c_default, _u(0x57), txdata, 1, true);
        if(ret == PICO_ERROR_GENERIC) {
            printf("Error Writing...\n");
        } else {
            sleep_ms(10);
            printf("Bytes written: %02x\n", ret);
            printf("Reading!\n");
            ret = i2c_read_blocking(i2c_default, 0x57, rxdata, 3, false);
            printf("Bytes read: %d\n", ret);
            printf("Data? %d\n", rxdata[0]);
            printf("Data? %d\n", rxdata[1]);
            printf("Data? %d\n", rxdata[2]);
        }
        sleep_ms(1000);
    }
    // // This example will use I2C0 on the default SDA and SCL pins (GP4, GP5 on a Pico)
    // // Make the I2C pins available to picotool
    // bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    // printf("\nI2C Bus Scan\n");
    // printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

    // for (int addr = 0; addr < (1 << 7); ++addr) {
    //     if (addr % 16 == 0) {
    //         printf("%02x ", addr);
    //     }

    //     // Perform a 1-byte dummy read from the probe address. If a slave
    //     // acknowledges this address, the function returns the number of bytes
    //     // transferred. If the address byte is ignored, the function returns
    //     // -1.

    //     // Skip over any reserved addresses.
    //     int ret;
    //     uint8_t rxdata;
    //     if (reserved_addr(addr))
    //         ret = PICO_ERROR_GENERIC;
    //     else
    //         ret = i2c_read_blocking(i2c_default, addr, &rxdata, 1, false);

    //     printf(ret < 0 ? "." : "@");
    //     printf(addr % 16 == 15 ? "\n" : "  ");
    // }
    // printf("Done.\n");
    // return 0;
}
