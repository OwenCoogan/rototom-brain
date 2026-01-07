#pragma once
#include <lvgl.h>

// Page IDs
enum PageID {
    PAGE_HOME = 0,
    PAGE_ROTOTOM_1 = 1,
    PAGE_ROTOTOM_2 = 2
};

// Initialize pages system
void pages_init();

// Navigate to a specific page
void pages_show_page(PageID page_id);

// Get current page ID
PageID pages_get_current();

// Get page screen object
lv_obj_t* pages_get_screen(PageID page_id);
