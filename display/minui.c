/**
 * @file minui.c
 *
 */

#define SURFACE_DATA_ALIGNMENT 8

/*********************
 *      INCLUDES
 *********************/
#include "minui.h"

#include <minui/minui.h>

#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <math.h>

#if USE_MINUI

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      STRUCTURES
 **********************/

struct minui_backend{
    gr_surface surface;
    uint32_t width;
    uint32_t height;
    uint32_t physical_width;
    uint32_t physical_height;
};

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static struct minui_backend backend;
static lv_area_t * old_area = NULL;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void minui_init(void)
{
    if (gr_init(true)) {
        perror("Error: unable to init minui");
        return;
    }
    LV_LOG_INFO("Minui initialized successfully");
  
    gr_color(0, 0, 0, 255);
    gr_clear();
  
    /* Obtain display info */
    backend.width = gr_fb_width();
    backend.height = gr_fb_height();
    backend.physical_width = gr_fb_mm_width();
    backend.physical_height = gr_fb_mm_height();

    /* Create surface */
    backend.surface = res_malloc_surface(backend.width * backend.height * 4) /* 4 channels */;
    backend.surface->row_bytes = backend.width * 4;
    backend.surface->pixel_bytes = 4;
    backend.surface->width = backend.width;
    backend.surface->height = backend.height;
}

void minui_exit(void)
{
    gr_fb_blank(true);
    gr_exit();

    if (old_area)
        free(old_area);
  
    if (backend.surface)
        res_free_surface(backend.surface);
}

/**
 * Flush a buffer to the marked area
 * @param drv pointer to driver where this function belongs
 * @param area an area where to copy `color_p`
 * @param color_p an array of pixels to copy to the `area` part of the screen
 */

void minui_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_p)
{
    int32_t x, y;
    uint32_t dst_p;
    lv_coord_t x1, y1;
    uint8_t color_tmp;

    if (old_area) {
      x1 = LV_MAX(old_area->x1 - 1, 0);
      y1 = LV_MAX(old_area->y1 - 1, 0);
      gr_blit(backend.surface, x1, y1, (old_area->x2 - x1), (old_area->y2 - y1), x1, y1);
    }

    for(y = area->y1; y <= area->y2; y++) {
        for(x = area->x1; x <= area->x2; x++) {
            dst_p = (y * backend.width * 4) + (x * 4);

#if MINUI_IS_BGRA
            color_tmp = color_p->ch.red;
            color_p->ch.red = color_p->ch.blue;
            color_p->ch.blue = color_tmp;
#endif /* MINUI_IS_BGRA */
            memcpy(&backend.surface->data[dst_p], &color_p->ch, sizeof(color_p->ch));
            color_p++;
        }
    }

    x1 = LV_MAX(area->x1 - 1, 0);
    y1 = LV_MAX(area->y1 - 1, 0);

    gr_blit(backend.surface, x1, y1, (area->x2 - x1), (area->y2 - y1), x1, y1);
    gr_flip();

    if (!old_area)
        old_area = calloc(1, sizeof(lv_area_t));
    lv_area_copy(old_area, area);

    lv_disp_flush_ready(drv);
}

void minui_get_sizes(uint32_t * width, uint32_t * height, uint32_t * dpi) {
    if (width)
        *width = backend.width;

    if (height)
        *height = backend.height;

    if (dpi)
        *dpi = round(backend.width / (backend.physical_width / 25.4));
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

#endif /* USE_MINUI */
