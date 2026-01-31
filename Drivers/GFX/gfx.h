#pragma once
#include <stdint.h>

void gfx_draw_mono(uint16_t x, uint16_t y,
                   uint16_t w, uint16_t h,
                   uint16_t bpr,
                   const uint8_t *bmp,
                   uint16_t fg, uint16_t bg);
