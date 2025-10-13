#include "pc_waker.h"
#include "pico/stdlib.h"
#include <device/usbd.h>
#include <class/hid/hid_device.h>
#include "cdc_output.h"
#include "util.h"

// Private helper functions
static bool can_wake(PCWaker* waker);

void pc_waker_init(PCWaker* waker) {
    waker->last_wake_time = 0;
    waker->debounce_ms = 5000;
    waker->current_keypress_duration = 50;
    waker->key_is_pressed = false;
    waker->last_keypress_time = 0;
    // Assumes TinyUSB is already initialized (do it in main c file)
}

bool pc_waker_is_awake(PCWaker* waker) {
    return !tud_suspended();
}

static bool can_wake(PCWaker* waker) {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    return (current_time - waker->last_wake_time) >= waker->debounce_ms;
}

bool pc_waker_wake(PCWaker* waker) {
    // Check debounce
    if (!can_wake(waker)) {
        uint32_t remaining = waker->debounce_ms - (to_ms_since_boot(get_absolute_time()) - waker->last_wake_time);
        cdc_print_debug("Debounce active, %lu ms remaining\n", remaining);
        return false;
    }
    
    // Try USB remote wakeup first
    if (tud_remote_wakeup()) {
        cdc_print_debug("USB remote wakeup sent\n");
        repeat_blink(5, 200); // To inform we're attempting a wake
        waker->last_wake_time = to_ms_since_boot(get_absolute_time());
        
        // Additionally press the space button for a second
        pc_waker_press_key(waker, HID_KEY_SPACE, 1000);
        return true;
    } else {
        cdc_print_debug("Remote wakeup failed. Most likely PC was awake\n");
        return false;
    }
}

void pc_waker_set_debounce_ms(PCWaker* waker, uint32_t ms) {
    waker->debounce_ms = ms;
}

void pc_waker_press_key(PCWaker* waker, uint8_t keycode, uint32_t duration) {
    if (!tud_hid_ready()) return;
    if (waker->key_is_pressed) return;
    
    // Key press
    uint8_t keycodes[6] = {0};
    keycodes[0] = keycode;
    tud_hid_keyboard_report(0, 0, keycodes);
    
    waker->last_keypress_time = to_ms_since_boot(get_absolute_time());
    waker->key_is_pressed = true;
    waker->current_keypress_duration = duration;
}

void pc_waker_release_keys(PCWaker* waker){
    if (!tud_hid_ready()) return;

    uint8_t keycodes[6] = {0};
    tud_hid_keyboard_report(0, 0, keycodes);
    
    waker->last_keypress_time = 0;
    waker->key_is_pressed = false;
    waker->current_keypress_duration = 50; // Default
}

void pc_waker_task(PCWaker* waker){
    if (!tud_hid_ready()) return;
    uint32_t now = to_ms_since_boot(get_absolute_time());

    if(!waker->key_is_pressed) return;

    if (now - waker->last_keypress_time > waker->current_keypress_duration){
        pc_waker_release_keys(waker);
    }
}


// TinyUSB callbacks for suspend/resume
void tud_suspend_cb(bool remote_wakeup_en) {
    cdc_print_debug("USB suspended (remote_wakeup: %d)\n", remote_wakeup_en);
    repeat_blink(3, 100);
}

void tud_resume_cb(void) {
    cdc_print_debug("USB resumed - PC wake up\n");
}

// HID callbacks (required by TinyUSB)
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, 
                                hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) {
    return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, 
                            hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
}