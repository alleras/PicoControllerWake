#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

void hal_led_toggle(void);
void blink(uint32_t ms);
void repeat_blink(uint8_t repeat, uint32_t ms);
void wait(int ms);
void process_blink(int blink_every_ms);

#endif // UTIL_H