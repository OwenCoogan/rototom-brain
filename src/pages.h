#pragma once
#include <lvgl.h>

enum PageID {
    PAGE_HOME = 0,
    PAGE_ROTOTOM_1 = 1,
    PAGE_ROTOTOM_2 = 2
};

void pages_init();
void pages_show_page(PageID page_id);
PageID pages_get_current();
lv_obj_t* pages_get_screen(PageID page_id);
