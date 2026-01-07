#include "display.h"
#include "pins.h"
#include <Arduino.h>
#include <Wire.h>
#include <lvgl.h>

// Include LovyanGFX - must define LGFX_USE_V1 before including
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
// Note: LGFX_Config.hpp also includes LovyanGFX.hpp, but #pragma once prevents double inclusion

// Define TFT colors for fillScreen
#define TFT_BLACK 0x0000

// In LovyanGFX v1, types are in the lgfx namespace
// Create a custom LGFX class for WT32-SC01 PLUS  
class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_ST7796 _panel_instance;  // WT32-SC01 PLUS uses ST7796, not ST7789
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
      cfg.i2c_addr = 0x38;  // FT6336U/FT5x06 default address (also try 0x48 or 0x14)
      cfg.pin_sda = TOUCH_SDA_PIN;
      cfg.pin_scl = TOUCH_SCL_PIN;
      cfg.freq = 400000;
      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
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

// Touchscreen input callback for LVGL
void my_touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data) {
    static int16_t last_x = 0, last_y = 0;
    static bool last_touched = false;
    static uint32_t last_debug_time = 0;
    static uint32_t call_count = 0;
    
    call_count++;
    // Debug: print on first call and every 5000 calls
    static bool first_call = true;
    if (first_call) {
        Serial.println("Touch callback first called - LVGL is polling for touch");
        first_call = false;
    }
    if (call_count % 5000 == 0) {
        Serial.printf("Touch callback called %lu times (still polling)\n", call_count);
    }
    
    // Get touch from display (LovyanGFX handles the touchscreen internally)
    bool touched = display.getTouch(&last_x, &last_y);
    
    if (touched) {
        // Map touch coordinates to display coordinates
        // Display is 320x480 in portrait mode (rotation 1)
        int16_t x = last_x;
        int16_t y = last_y;
        
        // Debug: print touch coordinates (throttled to avoid spam)
        uint32_t now = millis();
        if (!last_touched || (now - last_debug_time > 200)) {
            Serial.printf("Touch detected: x=%d, y=%d\n", x, y);
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
    
    // Initialize I2C FIRST (before display init) for touchscreen
    // FT6336U needs I2C to be ready before display.init() initializes it
    Serial.println("Initializing I2C for touchscreen (FT6336U)...");
    Serial.printf("I2C pins: SDA=%d, SCL=%d, INT=%d, RST=%d\n", 
                  TOUCH_SDA_PIN, TOUCH_SCL_PIN, TOUCH_INT_PIN, TOUCH_RST_PIN);
    Wire.begin(TOUCH_SDA_PIN, TOUCH_SCL_PIN);
    Wire.setClock(400000);  // Set I2C speed to 400kHz
    delay(200);  // Give I2C time to initialize
    
    // Initialize LovyanGFX display (parallel)
    Serial.println("Initializing LovyanGFX (parallel)...");
    display.init();
    // Get actual display dimensions after init
    int16_t display_width = display.width();
    int16_t display_height = display.height();
    Serial.printf("Display size: %dx%d\n", display_width, display_height);
    
    // Try different rotations to get 320x480 (portrait mode)
    // Rotations: 0=0°, 1=90°, 2=180°, 3=270°
    // For WT32-SC01 PLUS, we need 320x480 portrait
    int best_rotation = 1;
    display.setRotation(1);
    display_width = display.width();
    display_height = display.height();
    Serial.printf("Rotation 1: %dx%d\n", display_width, display_height);
    
    // Try rotation 0
    if (display_width != 320 || display_height != 480) {
        display.setRotation(0);
        display_width = display.width();
        display_height = display.height();
        Serial.printf("Rotation 0: %dx%d\n", display_width, display_height);
        if (display_width == 320 && display_height == 480) {
            best_rotation = 0;
        }
    }
    
    // Try rotation 2
    if (display_width != 320 || display_height != 480) {
        display.setRotation(2);
        display_width = display.width();
        display_height = display.height();
        Serial.printf("Rotation 2: %dx%d\n", display_width, display_height);
        if (display_width == 320 && display_height == 480) {
            best_rotation = 2;
        }
    }
    
    // Try rotation 3
    if (display_width != 320 || display_height != 480) {
        display.setRotation(3);
        display_width = display.width();
        display_height = display.height();
        Serial.printf("Rotation 3: %dx%d\n", display_width, display_height);
        if (display_width == 320 && display_height == 480) {
            best_rotation = 3;
        }
    }
    
    // Set to best rotation found
    display.setRotation(best_rotation);
    display_width = display.width();
    display_height = display.height();
    Serial.printf("Final rotation %d: %dx%d\n", best_rotation, display_width, display_height);
    
    // Fill entire screen with black
    display.fillScreen(TFT_BLACK);
    
    // Ensure backlight
    pinMode(LCD_BL_PIN, OUTPUT);
    digitalWrite(LCD_BL_PIN, HIGH);
    
    Serial.println("LovyanGFX initialized");
    
    // Note: I2C is initialized in the LGFX constructor for the touchscreen
    // Note: FT6336U touchscreen is initialized by LovyanGFX during display.init()
    // Quick test to see if touchscreen responds
    delay(300);  // Give touchscreen time to initialize
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

    // Initialize LVGL
    lv_init();
    
    // Register display FIRST (input device needs a display to be registered)
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, LV_HOR_RES_MAX * 20);
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &draw_buf;
    disp_drv.flush_cb = my_flush_cb;
    // Use actual display dimensions from LovyanGFX
    // Force 320x480 for LVGL even if display reports different dimensions
    disp_drv.hor_res = 320;
    disp_drv.ver_res = 480;
    Serial.printf("LVGL resolution set to: %dx%d (display reports %dx%d)\n", 
                  disp_drv.hor_res, disp_drv.ver_res, display.width(), display.height());
    lv_disp_t * disp = lv_disp_drv_register(&disp_drv);
    if (disp) {
        Serial.println("Display driver registered successfully");
    } else {
        Serial.println("ERROR: Failed to register display driver");
    }
    
    // Initialize touchscreen input for LVGL (AFTER display is registered)
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    indev_drv.disp = disp;  // Associate with the registered display
    lv_indev_t * indev = lv_indev_drv_register(&indev_drv);
    if (indev) {
        Serial.println("Touchscreen input device registered successfully");
    } else {
        Serial.println("ERROR: Failed to register touchscreen input device");
    }

    Serial.println("Display initialized!");
}
