#include "pages.h"
#include "midi.h"
#include <Arduino.h>
#include <lvgl.h>

static lv_obj_t* home_screen = nullptr;
static lv_obj_t* rototom1_screen = nullptr;
static lv_obj_t* rototom2_screen = nullptr;

static PageID current_page = PAGE_HOME;

static int rototom1_intensity = 100;
static int rototom2_intensity = 100;
static lv_obj_t* rototom1_intensity_label = nullptr;
static lv_obj_t* rototom2_intensity_label = nullptr;
static void btn_rototom1_clicked(lv_event_t* e) {
    pages_show_page(PAGE_ROTOTOM_1);
}

static void btn_rototom2_clicked(lv_event_t* e) {
    pages_show_page(PAGE_ROTOTOM_2);
}

static void btn_back_clicked(lv_event_t* e) {
    pages_show_page(PAGE_HOME);
}

static void btn_midi_clicked(lv_event_t* e) {
    Serial.println("MIDI button clicked");
    int intensity = (current_page == PAGE_ROTOTOM_1) ? rototom1_intensity : rototom2_intensity;
    midi_trigger(intensity);
}

static void intensity_slider_changed(lv_event_t* e) {
    lv_obj_t* slider = lv_event_get_target(e);
    int32_t value = lv_slider_get_value(slider);
    
    if (current_page == PAGE_ROTOTOM_1) {
        rototom1_intensity = value;
        if (rototom1_intensity_label) {
            lv_label_set_text_fmt(rototom1_intensity_label, "Intensity: %d", value);
        }
    } else if (current_page == PAGE_ROTOTOM_2) {
        rototom2_intensity = value;
        if (rototom2_intensity_label) {
            lv_label_set_text_fmt(rototom2_intensity_label, "Intensity: %d", value);
        }
    }
}

static void create_home_page() {
    home_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(home_screen, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(home_screen, LV_OPA_COVER, LV_PART_MAIN);
    // Remove padding for proper centering
    lv_obj_set_style_pad_all(home_screen, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(home_screen, 0, LV_PART_MAIN);
    
    lv_obj_t* title = lv_label_create(home_screen);
    lv_label_set_text(title, "ROTOTOM BRAIN");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);
    
    lv_obj_t* btn1 = lv_btn_create(home_screen);
    lv_obj_set_size(btn1, 250, 80);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -50);
    lv_obj_add_event_cb(btn1, btn_rototom1_clicked, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn1, btn_rototom1_clicked, LV_EVENT_PRESSED, NULL);
    
    lv_obj_t* btn1_label = lv_label_create(btn1);
    lv_label_set_text(btn1_label, "ROTOTOM 1");
    lv_obj_center(btn1_label);
    
    lv_obj_t* btn2 = lv_btn_create(home_screen);
    lv_obj_set_size(btn2, 250, 80);
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 50);
    lv_obj_add_event_cb(btn2, btn_rototom2_clicked, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn2, btn_rototom2_clicked, LV_EVENT_PRESSED, NULL);
    
    lv_obj_t* btn2_label = lv_label_create(btn2);
    lv_label_set_text(btn2_label, "ROTOTOM 2");
    lv_obj_center(btn2_label);
}

static void create_rototom1_page() {
    rototom1_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(rototom1_screen, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(rototom1_screen, LV_OPA_COVER, LV_PART_MAIN);
    // Remove padding for proper centering
    lv_obj_set_style_pad_all(rototom1_screen, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(rototom1_screen, 0, LV_PART_MAIN);
    
    // Back arrow button next to title (no outline, just arrow)
    lv_obj_t* btn_back = lv_btn_create(rototom1_screen);
    lv_obj_set_size(btn_back, 40, 40);
    lv_obj_align(btn_back, LV_ALIGN_TOP_LEFT, 20, 20);
    lv_obj_add_event_cb(btn_back, btn_back_clicked, LV_EVENT_CLICKED, NULL);
    
    // Remove button outline and background
    lv_obj_set_style_bg_opa(btn_back, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(btn_back, 0, LV_PART_MAIN);
    lv_obj_set_style_outline_width(btn_back, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(btn_back, 0, LV_PART_MAIN);
    
    lv_obj_t* btn_back_label = lv_label_create(btn_back);
    lv_label_set_text(btn_back_label, LV_SYMBOL_LEFT);
    lv_obj_set_style_text_color(btn_back_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(btn_back_label, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_center(btn_back_label);
    
    // Title centered at top
    lv_obj_t* title = lv_label_create(rototom1_screen);
    lv_label_set_text(title, "ROTOTOM 1");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);
    
    // MIDI button - centered horizontally, positioned in upper center
    lv_obj_t* btn_midi = lv_btn_create(rototom1_screen);
    lv_obj_set_size(btn_midi, 200, 60);
    lv_obj_align(btn_midi, LV_ALIGN_CENTER, 0, -40);
    lv_obj_add_event_cb(btn_midi, btn_midi_clicked, LV_EVENT_CLICKED, NULL);
    
    lv_obj_t* btn_midi_label = lv_label_create(btn_midi);
    lv_label_set_text(btn_midi_label, "MIDI");
    lv_obj_center(btn_midi_label);
    
    // Intensity label - centered
    rototom1_intensity_label = lv_label_create(rototom1_screen);
    lv_label_set_text_fmt(rototom1_intensity_label, "Intensity: %d", rototom1_intensity);
    lv_obj_set_style_text_color(rototom1_intensity_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(rototom1_intensity_label, LV_ALIGN_CENTER, 0, 20);
    
    // Slider - centered horizontally, wider for landscape
    lv_obj_t* slider = lv_slider_create(rototom1_screen);
    lv_obj_set_width(slider, 350);
    lv_slider_set_range(slider, 0, 127);
    lv_slider_set_value(slider, rototom1_intensity, LV_ANIM_OFF);
    lv_obj_align(slider, LV_ALIGN_CENTER, 0, 60);
    lv_obj_add_event_cb(slider, intensity_slider_changed, LV_EVENT_VALUE_CHANGED, NULL);
}

static void create_rototom2_page() {
    rototom2_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(rototom2_screen, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(rototom2_screen, LV_OPA_COVER, LV_PART_MAIN);
    // Remove padding for proper centering
    lv_obj_set_style_pad_all(rototom2_screen, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(rototom2_screen, 0, LV_PART_MAIN);
    
    // Back arrow button next to title (no outline, just arrow)
    lv_obj_t* btn_back = lv_btn_create(rototom2_screen);
    lv_obj_set_size(btn_back, 40, 40);
    lv_obj_align(btn_back, LV_ALIGN_TOP_LEFT, 20, 20);
    lv_obj_add_event_cb(btn_back, btn_back_clicked, LV_EVENT_CLICKED, NULL);
    
    // Remove button outline and background
    lv_obj_set_style_bg_opa(btn_back, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(btn_back, 0, LV_PART_MAIN);
    lv_obj_set_style_outline_width(btn_back, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(btn_back, 0, LV_PART_MAIN);
    
    lv_obj_t* btn_back_label = lv_label_create(btn_back);
    lv_label_set_text(btn_back_label, LV_SYMBOL_LEFT);
    lv_obj_set_style_text_color(btn_back_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(btn_back_label, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_center(btn_back_label);
    
    // Title centered at top
    lv_obj_t* title = lv_label_create(rototom2_screen);
    lv_label_set_text(title, "ROTOTOM 2");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);
    
    // MIDI button - centered horizontally, positioned in upper center
    lv_obj_t* btn_midi = lv_btn_create(rototom2_screen);
    lv_obj_set_size(btn_midi, 200, 60);
    lv_obj_align(btn_midi, LV_ALIGN_CENTER, 0, -40);
    lv_obj_add_event_cb(btn_midi, btn_midi_clicked, LV_EVENT_CLICKED, NULL);
    
    lv_obj_t* btn_midi_label = lv_label_create(btn_midi);
    lv_label_set_text(btn_midi_label, "MIDI");
    lv_obj_center(btn_midi_label);
    
    // Intensity label - centered
    rototom2_intensity_label = lv_label_create(rototom2_screen);
    lv_label_set_text_fmt(rototom2_intensity_label, "Intensity: %d", rototom2_intensity);
    lv_obj_set_style_text_color(rototom2_intensity_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(rototom2_intensity_label, LV_ALIGN_CENTER, 0, 20);
    
    // Slider - centered horizontally, wider for landscape
    lv_obj_t* slider = lv_slider_create(rototom2_screen);
    lv_obj_set_width(slider, 350);
    lv_slider_set_range(slider, 0, 127);
    lv_slider_set_value(slider, rototom2_intensity, LV_ANIM_OFF);
    lv_obj_align(slider, LV_ALIGN_CENTER, 0, 60);
    lv_obj_add_event_cb(slider, intensity_slider_changed, LV_EVENT_VALUE_CHANGED, NULL);
}

void pages_init() {
    Serial.println("Initializing pages...");
    
    create_home_page();
    Serial.println("Home page created");
    create_rototom1_page();
    Serial.println("Rototom 1 page created");
    create_rototom2_page();
    Serial.println("Rototom 2 page created");
    
    pages_show_page(PAGE_HOME);
    
    Serial.println("Pages initialized and home page displayed");
}

void pages_show_page(PageID page_id) {
    lv_obj_t* screen = nullptr;
    
    switch(page_id) {
        case PAGE_HOME:
            screen = home_screen;
            break;
        case PAGE_ROTOTOM_1:
            screen = rototom1_screen;
            break;
        case PAGE_ROTOTOM_2:
            screen = rototom2_screen;
            break;
        default:
            screen = home_screen;
            break;
    }
    
    if (screen != nullptr) {
        lv_scr_load(screen);
        current_page = page_id;
        Serial.printf("Switched to page %d (screen ptr: %p)\n", page_id, screen);
        lv_refr_now(NULL);
    } else {
        Serial.printf("ERROR: Screen is null for page %d\n", page_id);
    }
}

PageID pages_get_current() {
    return current_page;
}

lv_obj_t* pages_get_screen(PageID page_id) {
    switch(page_id) {
        case PAGE_HOME:
            return home_screen;
        case PAGE_ROTOTOM_1:
            return rototom1_screen;
        case PAGE_ROTOTOM_2:
            return rototom2_screen;
        default:
            return home_screen;
    }
}
