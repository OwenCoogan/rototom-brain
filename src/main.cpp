#include <Arduino.h>
#include <lvgl.h>
#include "display.h"
#include "pages.h"
#include "midi.h"
#include "pads.h"

void setup() {
    Serial.begin(115200);
    delay(200);
    Serial.println("\n=== Init display + LVGL ===");

    display_init();

    midi_init();

    pages_init();
    
    // TEMPORARILY DISABLED - Testing if pads_init causes black screen
    // pads_init();
    // pads_setHitCallback(pages_onPadHit);

    Serial.println("UI created");
}

void loop() {
    lv_timer_handler();
    // TEMPORARILY DISABLED - Testing if pads_update causes black screen
    // pads_update();  // Update piezo pad readings
    delay(5);
}
