#ifndef USER_SETUP_LOADED
#define USER_SETUP_LOADED

// WT32-SC01 PLUS - Parallel 8-bit configuration for TFT_eSPI
#define ST7796_DRIVER

// Screen size
#define TFT_WIDTH  320
#define TFT_HEIGHT 480

// Use 8-bit parallel MCU interface
#define TFT_PARALLEL_8_BIT

// Control pins
#define TFT_CS    -1
#define TFT_DC     0   // LCD_RS
#define TFT_RST    4   // LCD_RESET
#define TFT_WR    47   // LCD_WR
#define TFT_RD    -1

// Data pins DB0..DB7
#define TFT_D0     9
#define TFT_D1    46
#define TFT_D2     3
#define TFT_D3     8
#define TFT_D4    18
#define TFT_D5    17
#define TFT_D6    16
#define TFT_D7    15

// Backlight
#define TFT_BL    45

// Color order
#define TFT_RGB_ORDER TFT_BGR

#endif // USER_SETUP_LOADED
