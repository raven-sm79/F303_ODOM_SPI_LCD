#pragma once
#include <stdint.h>

void st7789_init(void);
void st7789_set_window(uint16_t x0, uint16_t y0,
                       uint16_t x1, uint16_t y1);
void st7789_write_pixel(uint16_t rgb565);
void st7789_write_pixels(const uint16_t *data, uint32_t count);
