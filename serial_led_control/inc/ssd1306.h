#ifndef SSD1306_H
#define SSD1306_H

#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Definições do display
#define SSD1306_WIDTH  128
#define SSD1306_HEIGHT 64
#define SSD1306_ADDRESS 0x3C

// Comandos do SSD1306 (valores hexadecimais)
#define SSD1306_SET_CONTRAST         0x81
#define SSD1306_SET_ENTIRE_ON        0xA4
#define SSD1306_SET_NORMAL_DISPLAY   0xA6
#define SSD1306_SET_DISPLAY_OFF      0xAE
#define SSD1306_SET_DISPLAY_ON       0xAF
#define SSD1306_SET_MEMORY_MODE      0x20
#define SSD1306_SET_COL_ADDR         0x21
#define SSD1306_SET_PAGE_ADDR        0x22
#define SSD1306_SET_DISPLAY_START    0x40
#define SSD1306_SET_SEG_REMAP        0xA0
#define SSD1306_SET_MUX_RATIO        0xA8
#define SSD1306_SET_COM_SCAN_DIR     0xC0
#define SSD1306_SET_DISPLAY_OFFSET   0xD3
#define SSD1306_SET_COM_PINS         0xDA
#define SSD1306_SET_DISPLAY_CLOCK    0xD5
#define SSD1306_SET_PRECHARGE        0xD9
#define SSD1306_SET_VCOM_DESELECT    0xDB
#define SSD1306_SET_CHARGE_PUMP      0x8D

// Estrutura principal do display
typedef struct {
    uint8_t width;          // Largura do display (128)
    uint8_t height;         // Altura do display (64)
    uint8_t pages;          // Número de páginas (8 para 64 pixels de altura)
    bool external_vcc;      // Fonte de alimentação externa (true/false)
    i2c_inst_t *i2c_port;   // Porta I2C (ex: i2c0 ou i2c1)
    uint8_t *buffer;        // Buffer de dados da tela
} ssd1306_t;

// Protótipos de funções
void ssd1306_init(ssd1306_t *ssd, i2c_inst_t *i2c_port); // Inicialização simplificada
void ssd1306_clear(ssd1306_t *ssd);                      // Limpa o buffer
void ssd1306_update(ssd1306_t *ssd);                     // Envia o buffer para o display
void ssd1306_draw_pixel(ssd1306_t *ssd, uint8_t x, uint8_t y, bool color); // Desenha um pixel
void ssd1306_draw_char(ssd1306_t *ssd, uint8_t x, uint8_t y, char ch);     // Desenha um caractere

#endif // SSD1306_H