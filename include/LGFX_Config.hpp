#pragma once
#define LGFX_USE_V1

#define LGFX_BOARD_HAS_PSRAM 1

// WT32-SC01 PLUS pin mapping
#define LGFX_PIN_SCK       -1
#define LGFX_PIN_MOSI      -1
#define LGFX_PIN_MISO      -1
#define LGFX_PIN_DC        0    // RS
#define LGFX_PIN_CS        -1
#define LGFX_PIN_RST       4
#define LGFX_PIN_BL        45

// Parallel pins
#define LGFX_PIN_D0        9
#define LGFX_PIN_D1        46
#define LGFX_PIN_D2        3
#define LGFX_PIN_D3        8
#define LGFX_PIN_D4        18
#define LGFX_PIN_D5        17
#define LGFX_PIN_D6        16
#define LGFX_PIN_D7        15
#define LGFX_PIN_WR        47
#define LGFX_PIN_RD        -1

#include <LovyanGFX.hpp>
