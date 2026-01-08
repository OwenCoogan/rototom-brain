#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/*====================
   GRAPHICS SETTINGS
 *====================*/
#define LV_COLOR_DEPTH 16
#define LV_COLOR_16_SWAP 0
#define LV_HOR_RES_MAX 320
#define LV_VER_RES_MAX 480

/*====================
   MEMORY SETTINGS
 *====================*/
#define LV_MEM_SIZE (48U * 1024U)
#define LV_MEM_CUSTOM 0

/*====================
   FEATURE CONFIGURATION
 *====================*/
#define LV_USE_LOG 0
#define LV_USE_ASSERT_NULL 0
#define LV_USE_ASSERT_MALLOC 0
#define LV_USE_ASSERT_STYLE 0
#define LV_USE_ASSERT_MEM_INTEGRITY 0
#define LV_USE_ASSERT_OBJ 0

#define LV_USE_PERF_MONITOR 0
#define LV_USE_ANIMATION 1
#define LV_USE_GPU 0
#define LV_USE_FILESYSTEM 0
#define LV_USE_USER_DATA 1

/*====================
   HAL SETTINGS
 *====================*/
#define LV_TICK_CUSTOM 1
#define LV_TICK_CUSTOM_INCLUDE "Arduino.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis())

/*====================
   OBJECT SETTINGS
 *====================*/
#define LV_USE_OBJ 1
#define LV_USE_LABEL 1
#define LV_USE_BTN 1
#define LV_USE_IMAGE 0
#define LV_USE_CHART 0
#define LV_USE_LINE 0
#define LV_USE_SLIDER 1
#define LV_USE_SWITCH 0
#define LV_USE_CHECKBOX 0
#define LV_USE_DROPDOWN 0
#define LV_USE_ROLLER 0
#define LV_USE_TEXTAREA 0
#define LV_USE_KEYBOARD 0
#define LV_USE_SPINBOX 0
#define LV_USE_SPINNER 0
#define LV_USE_TABLE 0
#define LV_USE_TABVIEW 0
#define LV_USE_TILEVIEW 0
#define LV_USE_WIN 0
#define LV_USE_MSGBOX 0
#define LV_USE_THEME_DEFAULT 1

/*====================
   EXTRA WIDGETS
 *====================*/
#define LV_USE_CALENDAR 0
#define LV_USE_COLORWHEEL 0
#define LV_USE_IMGARC 0
#define LV_USE_METER 0
#define LV_USE_MSGBOX_MNG 0
#define LV_USE_SPINBOX_MNG 0
#define LV_USE_TILEVIEW_MNG 0

#endif /*LV_CONF_H*/
