#include "st7789.h"
#include "main.h"
#include "spi.h"
#include "gpio.h"
#include "stm32f1xx_hal.h"

extern SPI_HandleTypeDef hspi1;

/* GPIO shortcuts */
static inline void CS_LOW(void)  { HAL_GPIO_WritePin(CS_GPIO_Port,  CS_Pin,  GPIO_PIN_RESET); }
static inline void CS_HIGH(void) { HAL_GPIO_WritePin(CS_GPIO_Port,  CS_Pin,  GPIO_PIN_SET);   }
static inline void DC_LOW(void)  { HAL_GPIO_WritePin(DC_GPIO_Port,  DC_Pin,  GPIO_PIN_RESET); }
static inline void DC_HIGH(void) { HAL_GPIO_WritePin(DC_GPIO_Port,  DC_Pin,  GPIO_PIN_SET);   }
static inline void RST_LOW(void) { HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_RESET); }
static inline void RST_HIGH(void){ HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_SET);   }

static void lcd_cmd(uint8_t c)
{
    DC_LOW();
    CS_LOW();
    HAL_SPI_Transmit(&hspi1, &c, 1, HAL_MAX_DELAY);
    CS_HIGH();
}

static void lcd_data(const uint8_t *d, uint16_t len)
{
    DC_HIGH();
    CS_LOW();
    HAL_SPI_Transmit(&hspi1, (uint8_t*)d, len, HAL_MAX_DELAY);
    CS_HIGH();
}

static void lcd_data8(uint8_t d)
{
    lcd_data(&d, 1);
}

static void lcd_data16(uint16_t d)
{
    uint8_t b[2] = { d >> 8, d & 0xFF };
    lcd_data(b, 2);
}

static void lcd_reset(void)
{
    RST_HIGH();
    HAL_Delay(5);
    RST_LOW();
    HAL_Delay(20);
    RST_HIGH();
    HAL_Delay(120);
}

void st7789_init(void)
{
    CS_HIGH();
    DC_HIGH();

    lcd_reset();

    lcd_cmd(0x01); HAL_Delay(150);   // SWRESET
    lcd_cmd(0x11); HAL_Delay(120);   // SLPOUT

    lcd_cmd(0x3A); lcd_data8(0x55);  // RGB565
    lcd_cmd(0x36); lcd_data8(0x08);  // MADCTL
    lcd_cmd(0x20);                   // INVON (если надо — 0x20)
    lcd_cmd(0x29); HAL_Delay(20);    // DISPON
}

void st7789_set_window(uint16_t x0, uint16_t y0,
                       uint16_t x1, uint16_t y1)
{
    uint8_t d[4];

    lcd_cmd(0x2A);
    d[0]=x0>>8; d[1]=x0&0xFF; d[2]=x1>>8; d[3]=x1&0xFF;
    lcd_data(d,4);

    lcd_cmd(0x2B);
    d[0]=y0>>8; d[1]=y0&0xFF; d[2]=y1>>8; d[3]=y1&0xFF;
    lcd_data(d,4);

    lcd_cmd(0x2C);
}

void st7789_write_pixel(uint16_t rgb565)
{
    lcd_data16(rgb565);
}

void st7789_write_pixels(const uint16_t *data, uint32_t count)
{
    DC_HIGH();
    CS_LOW();
    HAL_SPI_Transmit(&hspi1, (uint8_t*)data, count * 2, HAL_MAX_DELAY);
    CS_HIGH();
}
