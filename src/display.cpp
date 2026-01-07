#include "display.h"
#include "pins.h"
#include <Arduino.h>
#include <Wire.h>
#include <lvgl.h>

// Define TFT colors for fillScreen
#define TFT_BLACK 0x0000

// Include LovyanGFX - must define LGFX_USE_V1 before including
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
// Note: LGFX_Config.hpp also includes LovyanGFX.hpp, but #pragma once prevents double inclusion

// In LovyanGFX v1, types are in the lgfx namespace
// Create a custom LGFX class for WT32-SC01 PLUS  
class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_ST7796 _panel_instance;  // WT32-SC01 PLUS uses ST7796, not ST7789
  lgfx::Bus_Parallel8 _bus_instance;
  lgfx::Light_PWM _light_instance;

public:
  LGFX(void)
  {
    {
      auto cfg = _bus_instance.config();
      cfg.freq_write = 20000000;
      cfg.pin_wr = LCD_WR;
      cfg.pin_rd = -1;
      cfg.pin_rs = LCD_RS;  // DC/RS pin
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
      cfg.rgb_order = false;  // BGR order for WT32-SC01 PLUS (matches TFT_eSPI config)
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

    setPanel(&_panel_instance);
  }
};

// Create the display instance
static LGFX display;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 20];  // Increased buffer size for better rendering

void my_flush_cb(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    int32_t w = area->x2 - area->x1 + 1;
    int32_t h = area->y2 - area->y1 + 1;

    // Flush the display buffer to screen
    display.pushImage(area->x1, area->y1, w, h, (uint16_t*)color_p);
    lv_disp_flush_ready(disp);
}

void display_init() {
    Serial.println("=== Init display + LVGL ===");
    
    // Initialize LovyanGFX display (parallel)
    Serial.println("Initializing LovyanGFX (parallel)...");
    display.init();
    // Get actual display dimensions after init
    int16_t display_width = display.width();
    int16_t display_height = display.height();
    Serial.printf("Display size: %dx%d\n", display_width, display_height);
    
    // Try rotation 1 first (portrait mode for WT32-SC01 PLUS)
    // Rotations: 0=0°, 1=90°, 2=180°, 3=270°
    display.setRotation(1);
    display_width = display.width();
    display_height = display.height();
    Serial.printf("After rotation 1: %dx%d\n", display_width, display_height);
    
    // Fill entire screen with black
    display.fillScreen(TFT_BLACK);
    
    // Ensure backlight
    pinMode(LCD_BL_PIN, OUTPUT);
    digitalWrite(LCD_BL_PIN, HIGH);
    
    Serial.println("LovyanGFX initialized");

    // Initialize I2C for touch (if needed)
    Wire.begin(TOUCH_SDA_PIN, TOUCH_SCL_PIN);

    // Initialize LVGL
    lv_init();

    lv_disp_draw_buf_init(&draw_buf, buf, NULL, LV_HOR_RES_MAX * 20);
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &draw_buf;
    disp_drv.flush_cb = my_flush_cb;
    // Use actual display dimensions from LovyanGFX
    disp_drv.hor_res = display.width();
    disp_drv.ver_res = display.height();
    Serial.printf("LVGL resolution: %dx%d\n", disp_drv.hor_res, disp_drv.ver_res);
    lv_disp_drv_register(&disp_drv);

    Serial.println("Display initialized!");
}
