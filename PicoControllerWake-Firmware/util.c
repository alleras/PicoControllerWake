#include "util.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "tusb.h"

static int led_state = 0;

void hal_led_toggle(void) {
    led_state = 1 - led_state;
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_state);
}

void blink(uint32_t ms) {
    hal_led_toggle();
    sleep_ms(ms);
    hal_led_toggle();
    sleep_ms(ms);
}

void repeat_blink(uint8_t repeat, uint32_t ms) {
    for (uint8_t i = 0; i < repeat; i++) {
        blink(ms);
    }
}

void wait(int ms) {
    uint32_t start = to_ms_since_boot(get_absolute_time());
    while (to_ms_since_boot(get_absolute_time()) - start < ms) {
        tud_task();
        cyw43_arch_poll();
        sleep_ms(1);
    }
}

void process_blink(int blink_every_ms){
    static uint32_t last_blink = 0;
    uint32_t now = to_ms_since_boot(get_absolute_time());

    if (now - last_blink > blink_every_ms){
        hal_led_toggle();
        last_blink = now;
    }
}