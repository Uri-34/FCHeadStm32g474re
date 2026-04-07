/**
 * @file lv_conf.h
 * Конфигурация LVGL v8
 */

#ifndef LV_CONF_H
#define LV_CONF_H

/* === Обязательно в начале === */
#define LV_COLOR_DEPTH     16
#define LV_COLOR_16_SWAP   1

#define LV_TICK_CUSTOM     0
#define LV_TICK_CUSTOM_INCLUDE "mbed.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (0)

#include <stdint.h>

// #pragma message "✅ Using LVGL v8 - custom lv_conf.h"

/* === Основные настройки === */
#define LV_HOR_RES_MAX     (172)
#define LV_VER_RES_MAX     (320)
#define LV_MEM_SIZE        (32 * 1024)

#define LV_USE_LOG         1
#define LV_LOG_LEVEL       LV_LOG_LEVEL_INFO

/* === Дисплей === */
#define LV_DISP_DEF_REFR_PERIOD  30
#define LV_DISP_DRAW_BUF_SIZE    (172 * 10)

/* === Графика === */
#define LV_USE_DRAW_SW     1
#define LV_USE_GPU         0

/* === Виджеты === */
#define LV_USE_OBJ         1
#define LV_USE_IMG         1
#define LV_USE_LABEL       1
#define LV_USE_BTN         1

/* === Шрифты === */
#define LV_FONT_MONTSERRAT_14  1
#define LV_FONT_DEFAULT        &lv_font_montserrat_14

/* === Система === */
#define LV_USE_ANIMATION     1
#define LV_USE_USER_DATA     1

/* === Безопасность === */
#define LV_USE_ASSERT_NULL   1
#define LV_USE_ASSERT_OBJ    1
#define LV_USE_ASSERT_MEM    1

#endif /* LV_CONF_H */