#ifndef PC_WAKER_H
#define PC_WAKER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint32_t last_wake_time;
    uint32_t debounce_ms;
    uint32_t last_keypress_time;
    uint32_t current_keypress_duration;
    bool key_is_pressed;
} PCWaker;

void pc_waker_init(PCWaker* waker);
bool pc_waker_is_awake(PCWaker* waker);
bool pc_waker_wake(PCWaker* waker);
void pc_waker_set_debounce_ms(PCWaker* waker, uint32_t ms);
void pc_waker_press_key(PCWaker* waker, uint8_t keycode, uint32_t duration);
void pc_waker_release_keys(PCWaker* waker);
void pc_waker_task(PCWaker* waker);

#endif // PC_WAKER_H