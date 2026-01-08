#include "display.h"
#include "pins.h"
#include <Arduino.h>
#include <Wire.h>
#include <lvgl.h>

#define LGFX_USE_V1
#include <LovyanGFX.hpp>

#define TFT_BLACK 0x0000

class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_ST7796 _panel_instance;
  lgfx::Bus_Parallel8 _bus_instance;
  lgfx::Light_PWM _light_instance;
  lgfx::Touch_FT5x06 _touch_instance;

public:
  LGFX(void)
  {
    {
      auto cfg = _bus_instance.config();
      cfg.freq_write = 20000000;
      cfg.pin_wr = LCD_WR;
      cfg.pin_rd = -1;
      cfg.pin_rs = LCD_RS;
      cfg.pin_d0 = LCD_DB0;
      cfg.pin_d1 = LCD_DB1;
      cfg.pin_d2 = LCD_DB2;
      cfg.pin_d3 = LCD_DB3;
      cfg.pin_d4 = LCD_DB4;
      cfg.pin_d5 = LCD_DB5;
      cfg.pin_d6 = LCD_DB6;
      cfg.pin_d7 = LCD_DB7;
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    {
      auto cfg = _panel_instance.config();
      cfg.pin_cs = -1;
      cfg.pin_rst = LCD_RST;
      cfg.pin_busy = -1;
      cfg.memory_width = 320;
      cfg.memory_height = 480;
      cfg.panel_width = 320;
      cfg.panel_height = 480;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits = 1;
      cfg.readable = true;
      cfg.invert = false;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = false;
      _panel_instance.config(cfg);
    }

    {
      auto cfg = _light_instance.config();
      cfg.pin_bl = LCD_BL_PIN;
      cfg.invert = false;
      cfg.freq = 44100;
      cfg.pwm_channel = 7;
      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);
    }

    {
      auto cfg = _touch_instance.config();
      cfg.x_min = 0;
      cfg.x_max = 319;
      cfg.y_min = 0;
      cfg.y_max = 479;
      cfg.pin_int = TOUCH_INT_PIN;
      cfg.bus_shared = true;
      cfg.offset_rotation = 0;
      cfg.i2c_port = 1;
      cfg.i2c_addr = 0x38;
      cfg.pin_sda = TOUCH_SDA_PIN;
      cfg.pin_scl = TOUCH_SCL_PIN;
      cfg.freq = 400000;
      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
    }

    setPanel(&_panel_instance);
  }
};

static LGFX display;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 20];

void my_flush_cb(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    int32_t w = area->x2 - area->x1 + 1;
    int32_t h = area->y2 - area->y1 + 1;

    display.pushImage(area->x1, area->y1, w, h, (uint16_t*)color_p);
    lv_disp_flush_ready(disp);
}

// Store the display rotation for touch coordinate mapping
static int display_rotation = 1;

void my_touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data) {
    static int16_t last_x = 0, last_y = 0;
    static bool last_touched = false;
    static uint32_t last_debug_time = 0;
    static uint32_t call_count = 0;
    
    call_count++;
    static bool first_call = true;
    if (first_call) {
        Serial.println("Touch callback first called - LVGL is polling for touch");
        first_call = false;
    }
    if (call_count % 5000 == 0) {
        Serial.printf("Touch callback called %lu times (still polling)\n", call_count);
    }
    
    bool touched = display.getTouch(&last_x, &last_y);
    
    if (touched) {
        // Transform touch coordinates based on display rotation
        // Touch hardware reports coordinates in native orientation
        // Need to transform based on how display is rotated
        int16_t x, y;
        int16_t current_width = display.width();
        int16_t current_height = display.height();
        
        // Transform coordinates based on rotation
        // Rotation 0: 480x320 (landscape) - swap x/y and invert y
        // Rotation 1: 320x480 (portrait) - use as-is
        // Rotation 2: 480x320 (landscape) - swap x/y and invert both
        // Rotation 3: 320x480 (portrait) - swap x/y
        switch(display_rotation) {
            case 0: // Landscape 480x320
                x = last_y;
                y = current_height - last_x;
                break;
            case 1: // Portrait 320x480
                x = last_x;
                y = last_y;
                break;
            case 2: // Landscape 480x320 (flipped)
                x = current_width - last_y;
                y = current_height - last_x;
                break;
            case 3: // Portrait 320x480 (flipped)
                x = current_width - last_x;
                y = last_y;
                break;
            default:
                x = last_x;
                y = last_y;
                break;
        }
        
        // Clamp to display bounds
        x = constrain(x, 0, current_width - 1);
        y = constrain(y, 0, current_height - 1);
        
        uint32_t now = millis();
        if (!last_touched || (now - last_debug_time > 200)) {
            Serial.printf("Touch detected: raw=(%d,%d) mapped=(%d,%d) rotation=%d display=%dx%d\n", 
                         last_x, last_y, x, y, display_rotation, current_width, current_height);
            last_debug_time = now;
        }
        
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = x;
        data->point.y = y;
        last_touched = true;
    } else {
        if (last_touched) {
            Serial.println("Touch released");
        }
        data->state = LV_INDEV_STATE_RELEASED;
        last_touched = false;
    }
}

void display_init() {
    Serial.println("=== Init display + LVGL ===");
    
    Serial.println("Initializing I2C for touchscreen (FT6336U)...");
    Serial.printf("I2C pins: SDA=%d, SCL=%d, INT=%d, RST=%d\n", 
                  TOUCH_SDA_PIN, TOUCH_SCL_PIN, TOUCH_INT_PIN, TOUCH_RST_PIN);
    Wire.begin(TOUCH_SDA_PIN, TOUCH_SCL_PIN);
    Wire.setClock(400000);
    delay(200);
    
    Serial.println("Initializing LovyanGFX (parallel)...");
    display.init();
    int16_t display_width = display.width();
    int16_t display_height = display.height();
    Serial.printf("Display size: %dx%d\n", display_width, display_height);
    
    // Find landscape orientation (480x320, width > height)
    int best_rotation = 1;
    display.setRotation(1);
    display_width = display.width();
    display_height = display.height();
    Serial.printf("Rotation 1: %dx%d\n", display_width, display_height);
    
    if (display_width != 480 || display_height != 320) {
        display.setRotation(0);
        display_width = display.width();
        display_height = display.height();
        Serial.printf("Rotation 0: %dx%d\n", display_width, display_height);
        if (display_width == 480 && display_height == 320) {
            best_rotation = 0;
        }
    }
    
    if (display_width != 480 || display_height != 320) {
        display.setRotation(2);
        display_width = display.width();
        display_height = display.height();
        Serial.printf("Rotation 2: %dx%d\n", display_width, display_height);
        if (display_width == 480 && display_height == 320) {
            best_rotation = 2;
        }
    }
    
    if (display_width != 480 || display_height != 320) {
        display.setRotation(3);
        display_width = display.width();
        display_height = display.height();
        Serial.printf("Rotation 3: %dx%d\n", display_width, display_height);
        if (display_width == 480 && display_height == 320) {
            best_rotation = 3;
        }
    }
    
    display.setRotation(best_rotation);
    display_rotation = best_rotation;  // Store rotation for touch coordinate mapping
    display_width = display.width();
    display_height = display.height();
    Serial.printf("Final rotation %d: %dx%d (LANDSCAPE)\n", best_rotation, display_width, display_height);
    
    display.fillScreen(TFT_BLACK);
    
    pinMode(LCD_BL_PIN, OUTPUT);
    digitalWrite(LCD_BL_PIN, HIGH);
    
    Serial.println("LovyanGFX initialized");
    
    delay(300);
    Serial.println("Testing touchscreen (FT6336U on SDA=6, SCL=5)...");
    int16_t tx, ty;
    bool touch_works = false;
    for (int i = 0; i < 3; i++) {
        if (display.getTouch(&tx, &ty)) {
            Serial.printf("Touchscreen responds! Test %d: x=%d, y=%d\n", i, tx, ty);
            touch_works = true;
            break;
        }
        delay(100);
    }
    if (!touch_works) {
        Serial.println("Touchscreen not responding - will try to work anyway");
    }

    lv_init();
    
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, LV_HOR_RES_MAX * 20);
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &draw_buf;
    disp_drv.flush_cb = my_flush_cb;
    // Set LVGL resolution to match landscape orientation (480x320)
    disp_drv.hor_res = display_width;  // 480 for landscape
    disp_drv.ver_res = display_height; // 320 for landscape
    Serial.printf("LVGL resolution set to: %dx%d (LANDSCAPE - display reports %dx%d)\n", 
                  disp_drv.hor_res, disp_drv.ver_res, display.width(), display.height());
    lv_disp_t * disp = lv_disp_drv_register(&disp_drv);
    if (disp) {
        Serial.println("Display driver registered successfully");
    } else {
        Serial.println("ERROR: Failed to register display driver");
    }
    
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    indev_drv.disp = disp;
    lv_indev_t * indev = lv_indev_drv_register(&indev_drv);
    if (indev) {
        Serial.println("Touchscreen input device registered successfully");
    } else {
        Serial.println("ERROR: Failed to register touchscreen input device");
    }

    Serial.println("Display initialized!");
}
