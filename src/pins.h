// WT32-SC01 PLUS pin mapping (from datasheet)
#ifndef PINS_H
#define PINS_H

// LCD parallel data pins
#define LCD_DB0 9
#define LCD_DB1 46
#define LCD_DB2 3
#define LCD_DB3 8
#define LCD_DB4 18
#define LCD_DB5 17
#define LCD_DB6 16
#define LCD_DB7 15

// Control pins
#define LCD_RS 0   // DC / RS
#define LCD_RST 4
#define LCD_WR 47
#define LCD_TE 48
#define LCD_BL_PIN 45  // backlight PWM

// Touch (I2C)
#define TOUCH_SDA_PIN 6
#define TOUCH_SCL_PIN 5
#define TOUCH_INT_PIN 7
#define TOUCH_RST_PIN 4

// Piezo pads (Extended IO connector)
// NOTE: Only GPIO1-3 are safe to use. GPIO4=LCD_RST, GPIO5=TOUCH_SCL, GPIO6=TOUCH_SDA
#define PIEZO_TOM1_PIN 1  // IO1 / GPIO1 (ADC) - OK, this is what we need
#define PIEZO_TOM2_PIN 2  // IO2 / GPIO2 (ADC) - OK
#define PIEZO_TOM3_PIN 3  // IO3 / GPIO3 (ADC) - OK
// GPIO4 conflicts with LCD_RST - DO NOT USE (causes black screen!)
// GPIO5 conflicts with TOUCH_SCL - DO NOT USE
// GPIO6 conflicts with TOUCH_SDA - DO NOT USE
#define PIEZO_TOM4_PIN 2  // Temporarily use GPIO2 (will be unused)
#define PIEZO_TOM5_PIN 2  // Temporarily use GPIO2 (will be unused)
#define PIEZO_TOM6_PIN 2  // Temporarily use GPIO2 (will be unused)

#endif // PINS_H
