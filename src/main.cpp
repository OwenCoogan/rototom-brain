#include <Arduino.h>
#include <lvgl.h>
#include "display.h"

void setup() {
    Serial.begin(115200);
    delay(200);
    Serial.println("\n=== Init display + LVGL ===");

    display_init();

    // Create a simple label
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Hello LVGL on WT32-SC01 PLUS");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    Serial.println("UI created");
}

void loop() {
    lv_timer_handler();
    delay(5);
}
