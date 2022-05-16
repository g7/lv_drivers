/**
 * @file minui.h
 *
 */

#ifndef MINUI_H
#define MINUI_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#ifndef LV_DRV_NO_CONF
#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lv_drv_conf.h"
#else
#include "../../lv_drv_conf.h"
#endif
#endif

#if USE_MINUI

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void minui_init(void);
void minui_exit(void);
void minui_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_p);
void minui_get_sizes(uint32_t *width, uint32_t *height, uint32_t *dpi);


/**********************
 *      MACROS
 **********************/

#endif  /*USE_MINUI*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*FBDEV_H*/
