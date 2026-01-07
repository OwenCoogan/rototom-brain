#pragma once

// Workaround for LovyanGFX GPIO compatibility with ESP32-S3
#include <soc/gpio_struct.h>
#include <soc/gpio_reg.h>

// Ensure GPIO is available as a global
extern gpio_dev_t GPIO;
