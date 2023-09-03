#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// I2C reserves some addresses for special purposes. We exclude these from the scan.
// These are any addresses of the form 000 0xxx or 111 1xxx
// bool reserved_addr(uint8_t addr) {
//     return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
// }

static char event_str[128];

void gpio_event_string(char *buf, uint32_t events);

void print_hello(uint gpio, uint32_t events) {
    gpio_event_string(event_str, events);
    printf("GPIO %d %s\n", gpio, event_str);
}

int main() {
    // Turn on the LED!
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);

    // Enable UART so we can print status output
    stdio_init_all();

    // Setup Ultra Pins
    printf("Hello GPIO IRQ\n");
    gpio_init(4);
    gpio_init(5);
    gpio_set_dir(4, GPIO_OUT);
    gpio_set_dir(4, GPIO_IN);
    gpio_pull_up(4);
    gpio_pull_up(5);
    gpio_set_irq_enabled_with_callback(4, GPIO_IRQ_EDGE_FALL, true, &print_hello);


    // Enter the main loop
    while(true) {}
}


static const char *gpio_irq_str[] = {
        "LEVEL_LOW",  // 0x1
        "LEVEL_HIGH", // 0x2
        "EDGE_FALL",  // 0x4
        "EDGE_RISE"   // 0x8
};

void gpio_event_string(char *buf, uint32_t events) {
    for (uint i = 0; i < 4; i++) {
        uint mask = (1 << i);
        if (events & mask) {
            // Copy this event string into the user string
            const char *event_str = gpio_irq_str[i];
            while (*event_str != '\0') {
                *buf++ = *event_str++;
            }
            events &= ~mask;

            // If more events add ", "
            if (events) {
                *buf++ = ',';
                *buf++ = ' ';
            }
        }
    }
    *buf++ = '\0';
}
