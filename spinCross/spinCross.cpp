#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

#define AUDIO_OUT_PIN 18
#define ANALOG_KNOB_PIN 26

int main() {
    stdio_init_all();

    // Setup ADC on GP26
    adc_init();
    adc_gpio_init(ANALOG_KNOB_PIN);
    adc_select_input(0);

    // Setup PWM on GP18
    gpio_set_function(AUDIO_OUT_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(AUDIO_OUT_PIN);
    pwm_set_wrap(slice_num, 3);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 1);
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 3);

    // Turn on the LED!
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    sleep_ms(1000);

    while(true) {
        const float conversion_factor = 3.3f / (1 << 12);
        uint16_t result = adc_read();
        float result_percentage = (float)result / (float)0xFFF;
        printf("Raw value: 0x%03x, voltage: %f V, percent: %f%\n", result, result * conversion_factor, result_percentage);
        gpio_put(PICO_DEFAULT_LED_PIN, result_percentage > 0.5);
        pwm_set_enabled(slice_num, result_percentage > 0.5);
        sleep_ms(500);
    }

}