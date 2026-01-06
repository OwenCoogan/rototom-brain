#include <Arduino.h>
#include <lvgl.h>
#include "display.h"
#include "pads.h"
#include "midi.h"
#include "buttons.h"

// LVGL objects
lv_obj_t *tom1_meter;
lv_obj_t *tom2_meter;
lv_obj_t *tom3_meter;

lv_obj_t *kit_label;

lv_obj_t *btn_prev;
lv_obj_t *btn_next;

int kit_number = 1;

void update_velocity_meters() {
    // Generate fake velocities for now
    int v1 = random(0, 127);
    int v2 = random(0, 127);
    int v3 = random(0, 127);

    lv_bar_set_value(tom1_meter, v1, LV_ANIM_OFF);
    lv_bar_set_value(tom2_meter, v2, LV_ANIM_OFF);
    lv_bar_set_value(tom3_meter, v3, LV_ANIM_OFF);
}

void btn_prev_event(lv_event_t *e) {
    kit_number--;
    if(kit_number < 1) kit_number = 5; // wrap around 1–5
    char buf[16];
    sprintf(buf, "Kit %d", kit_number);
    lv_label_set_text(kit_label, buf);
    Serial.printf("Previous Kit: %d\n", kit_number);
}

void btn_next_event(lv_event_t *e) {
    kit_number++;
    if(kit_number > 5) kit_number = 1; // wrap around 1–5
    char buf[16];
    sprintf(buf, "Kit %d", kit_number);
    lv_label_set_text(kit_label, buf);
    Serial.printf("Next Kit: %d\n", kit_number);
}

void setup() {
    Serial.begin(115200);
    randomSeed(esp_random());

    display_init();   // Initialize LVGL + TFT + dummy touch
    pads_init();
    midi_init();
    buttons_init();

    // ---- KIT LABEL ----
    kit_label = lv_label_create(lv_scr_act());
    lv_label_set_text(kit_label, "Kit 1");
    lv_obj_align(kit_label, LV_ALIGN_TOP_MID, 0, 10);

    // ---- VELOCITY METERS ----
    tom1_meter = lv_bar_create(lv_scr_act());
    lv_obj_set_size(tom1_meter, 30, 150);
    lv_obj_align(tom1_meter, LV_ALIGN_LEFT_MID, 40, 0);
    lv_bar_set_range(tom1_meter, 0, 127);
    lv_bar_set_value(tom1_meter, 0, LV_ANIM_OFF);

    tom2_meter = lv_bar_create(lv_scr_act());
    lv_obj_set_size(tom2_meter, 30, 150);
    lv_obj_align(tom2_meter, LV_ALIGN_CENTER, 0, 0);
    lv_bar_set_range(tom2_meter, 0, 127);
    lv_bar_set_value(tom2_meter, 0, LV_ANIM_OFF);

    tom3_meter = lv_bar_create(lv_scr_act());
    lv_obj_set_size(tom3_meter, 30, 150);
    lv_obj_align(tom3_meter, LV_ALIGN_RIGHT_MID, -40, 0);
    lv_bar_set_range(tom3_meter, 0, 127);
    lv_bar_set_value(tom3_meter, 0, LV_ANIM_OFF);

    // ---- BUTTONS ----
    btn_prev = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn_prev, 100, 40);
    lv_obj_align(btn_prev, LV_ALIGN_BOTTOM_LEFT, 20, -20);
    lv_obj_add_event_cb(btn_prev, btn_prev_event, LV_EVENT_CLICKED, NULL);
    lv_obj_t *prev_label = lv_label_create(btn_prev);
    lv_label_set_text(prev_label, "Prev Kit");
    lv_obj_center(prev_label);

    btn_next = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn_next, 100, 40);
    lv_obj_align(btn_next, LV_ALIGN_BOTTOM_RIGHT, -20, -20);
    lv_obj_add_event_cb(btn_next, btn_next_event, LV_EVENT_CLICKED, NULL);
    lv_obj_t *next_label = lv_label_create(btn_next);
    lv_label_set_text(next_label, "Next Kit");
    lv_obj_center(next_label);
}

void loop() {
    lv_timer_handler();   // LVGL update
    delay(20);

    update_velocity_meters(); // simulate tom velocities

    pads_update();       // placeholder for pads
    buttons_update();    // placeholder for buttons
}
