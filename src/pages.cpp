#include "pages.h"
#include <Arduino.h>
#include <lvgl.h>

// Screen objects for each page
static lv_obj_t* home_screen = nullptr;
static lv_obj_t* rototom1_screen = nullptr;
static lv_obj_t* rototom2_screen = nullptr;

static PageID current_page = PAGE_HOME;

// Button event handlers
static void btn_rototom1_clicked(lv_event_t* e) {
    pages_show_page(PAGE_ROTOTOM_1);
}

static void btn_rototom2_clicked(lv_event_t* e) {
    pages_show_page(PAGE_ROTOTOM_2);
}

static void btn_back_clicked(lv_event_t* e) {
    pages_show_page(PAGE_HOME);
}

// Create home page
static void create_home_page() {
    home_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(home_screen, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(home_screen, LV_OPA_COVER, LV_PART_MAIN);
    
    // Title
    lv_obj_t* title = lv_label_create(home_screen);
    lv_label_set_text(title, "ROTOTOM BRAIN");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    // Use larger font if available
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 30);
    
    // Button for Rototom 1
    lv_obj_t* btn1 = lv_btn_create(home_screen);
    lv_obj_set_size(btn1, 250, 80);  // Larger buttons for easier touch
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -60);
    lv_obj_add_event_cb(btn1, btn_rototom1_clicked, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn1, btn_rototom1_clicked, LV_EVENT_PRESSED, NULL);  // Also on press
    
    lv_obj_t* btn1_label = lv_label_create(btn1);
    lv_label_set_text(btn1_label, "ROTOTOM 1");
    lv_obj_center(btn1_label);
    
    // Button for Rototom 2
    lv_obj_t* btn2 = lv_btn_create(home_screen);
    lv_obj_set_size(btn2, 250, 80);  // Larger buttons for easier touch
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 60);
    lv_obj_add_event_cb(btn2, btn_rototom2_clicked, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn2, btn_rototom2_clicked, LV_EVENT_PRESSED, NULL);  // Also on press
    
    lv_obj_t* btn2_label = lv_label_create(btn2);
    lv_label_set_text(btn2_label, "ROTOTOM 2");
    lv_obj_center(btn2_label);
}

// Create Rototom 1 page
static void create_rototom1_page() {
    rototom1_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(rototom1_screen, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(rototom1_screen, LV_OPA_COVER, LV_PART_MAIN);
    
    // Title
    lv_obj_t* title = lv_label_create(rototom1_screen);
    lv_label_set_text(title, "ROTOTOM 1");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 30);
    
    // Back button
    lv_obj_t* btn_back = lv_btn_create(rototom1_screen);
    lv_obj_set_size(btn_back, 150, 50);
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_MID, 0, -30);
    lv_obj_add_event_cb(btn_back, btn_back_clicked, LV_EVENT_CLICKED, NULL);
    
    lv_obj_t* btn_back_label = lv_label_create(btn_back);
    lv_label_set_text(btn_back_label, "BACK");
    lv_obj_center(btn_back_label);
    
    // Placeholder for rototom 1 content
    lv_obj_t* content = lv_label_create(rototom1_screen);
    lv_label_set_text(content, "Rototom 1 Controls\n\n(Add your controls here)");
    lv_obj_set_style_text_color(content, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_align(content, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(content, LV_ALIGN_CENTER, 0, 0);
}

// Create Rototom 2 page
static void create_rototom2_page() {
    rototom2_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(rototom2_screen, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(rototom2_screen, LV_OPA_COVER, LV_PART_MAIN);
    
    // Title
    lv_obj_t* title = lv_label_create(rototom2_screen);
    lv_label_set_text(title, "ROTOTOM 2");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 30);
    
    // Back button
    lv_obj_t* btn_back = lv_btn_create(rototom2_screen);
    lv_obj_set_size(btn_back, 150, 50);
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_MID, 0, -30);
    lv_obj_add_event_cb(btn_back, btn_back_clicked, LV_EVENT_CLICKED, NULL);
    
    lv_obj_t* btn_back_label = lv_label_create(btn_back);
    lv_label_set_text(btn_back_label, "BACK");
    lv_obj_center(btn_back_label);
    
    // Placeholder for rototom 2 content
    lv_obj_t* content = lv_label_create(rototom2_screen);
    lv_label_set_text(content, "Rototom 2 Controls\n\n(Add your controls here)");
    lv_obj_set_style_text_color(content, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_align(content, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(content, LV_ALIGN_CENTER, 0, 0);
}

void pages_init() {
    Serial.println("Initializing pages...");
    
    // Create all pages
    create_home_page();
    Serial.println("Home page created");
    create_rototom1_page();
    Serial.println("Rototom 1 page created");
    create_rototom2_page();
    Serial.println("Rototom 2 page created");
    
    // Show home page initially
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
        // Force a refresh
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
