#include <Arduino.h>
#include <lvgl.h>
#include "display.h"
#include "pages.h"
#include "midi.h"

void setup() {
    Serial.begin(115200);
    delay(200);
    Serial.println("\n=== Init display + LVGL ===");

    display_init();

    midi_init();

    pages_init();

    Serial.println("UI created");
}

void loop() {
    lv_timer_handler();
    delay(5);
}
