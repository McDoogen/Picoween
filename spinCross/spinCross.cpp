#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "audio/dun.h"

#define AUDIO_OUT_PIN 18
#define ANALOG_KNOB_PIN 26


// Default clock is 125 MHz
// Playback frequency is 44.1KHz
// 125Mhz / 44.1KHz ~ 2834 counts before reset
const uint16_t WAV_COUNT = 2834;
uint32_t audio_step = 0;

void audio_wrap() {
    uint slice_num = pwm_gpio_to_slice_num(AUDIO_OUT_PIN);
    uint chan_num = pwm_gpio_to_channel(AUDIO_OUT_PIN);
    pwm_clear_irq(slice_num);

    if(audio_step < dun_wav_len)
        audio_step++;

    float step_percentage = (float)dun_wav[audio_step] / 255.0f;
    pwm_set_chan_level(slice_num, chan_num, WAV_COUNT*step_percentage);
}


int main() {
    stdio_init_all();

    // Setup ADC on GP26
    adc_init();
    adc_gpio_init(ANALOG_KNOB_PIN);
    adc_select_input(0);

    // Setup PWM on GP18
    gpio_set_function(AUDIO_OUT_PIN, GPIO_FUNC_PWM);
    // Get the slice and channel numbers for the GPIO pin we are using
    uint slice_num = pwm_gpio_to_slice_num(AUDIO_OUT_PIN);
    uint chan_num = pwm_gpio_to_channel(AUDIO_OUT_PIN);
    // This is the highest value we will count to before rolling to 0 or counting down
    pwm_set_wrap(slice_num, WAV_COUNT);
    // False to disable counting down and instead roll over to 0
    pwm_set_phase_correct(AUDIO_OUT_PIN, false);
    // This is the count when we swap the GPIO output level. For 50% duty cycle, it should be half of the max count
    pwm_set_chan_level(slice_num, chan_num, WAV_COUNT * 0.5);
    // Enables or disabled the PWM signal counting
    pwm_set_enabled(slice_num, false);

    // Setup PWM IRQ
    pwm_clear_irq(slice_num);
    pwm_set_irq_enabled(slice_num, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, audio_wrap);
    irq_set_enabled(PWM_IRQ_WRAP, true);
    
    // Turn on the LED!
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    sleep_ms(1000);

    audio_step = 0;
    while(true) {
        const float conversion_factor = 3.3f / (1 << 12);
        uint16_t result = adc_read();
        float result_percentage = (float)result / (float)0xFFF;
        // printf("Raw value: 0x%03x, voltage: %f V, percent: %f%\n", result, result * conversion_factor, result_percentage);
        if(result_percentage > 0.7) {
            gpio_put(PICO_DEFAULT_LED_PIN, true);
            pwm_set_enabled(slice_num, true);
        } else if (result_percentage < 0.3) {
            gpio_put(PICO_DEFAULT_LED_PIN, false);
            pwm_set_enabled(slice_num, false);
            audio_step= 0;
        }
    }

}