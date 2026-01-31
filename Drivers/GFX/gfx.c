#include "gfx.h"
#include "../LCD/lcd.h"

void gfx_draw_mono(uint16_t x, uint16_t y,
                   uint16_t w, uint16_t h,
                   uint16_t bpr,
                   const uint8_t *bmp,
                   uint16_t fg, uint16_t bg)
{
    lcd_draw_mono_1bpp(x, y, w, h, bmp, bpr, fg, bg);
}
