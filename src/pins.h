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

#endif // PINS_H
