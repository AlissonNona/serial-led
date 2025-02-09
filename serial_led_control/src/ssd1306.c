#include "ssd1306.h"
#include "font.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include <string.h>

#define SSD1306_I2C_ADDR 0x3C
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64

#define COLOR_ON 1

static uint8_t buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

// Envia comandos ao display via I2C
static void ssd1306_send_command(uint8_t cmd) {
    uint8_t buf[2] = {0x00, cmd}; // 0x00 = Co bit (command)
    i2c_write_blocking(i2c_default, SSD1306_I2C_ADDR, buf, 2, false);
}

// Inicialização do display
void ssd1306_init() {
    // Sequência de comandos de inicialização
    ssd1306_send_command(0xAE); // Display OFF
    ssd1306_send_command(0x20); // Set Memory Addressing Mode
    ssd1306_send_command(0x00); // Horizontal Addressing Mode
    ssd1306_send_command(0xB0); // Set Page Start Address
    ssd1306_send_command(0xC8); // Set COM Output Scan Direction
    ssd1306_send_command(0x00); // Set Low Column Address
    ssd1306_send_command(0x10); // Set High Column Address
    ssd1306_send_command(0x40); // Set Start Line Address
    ssd1306_send_command(0x81); // Set Contrast Control
    ssd1306_send_command(0xFF); // Contrast Value
    ssd1306_send_command(0xA1); // Set Segment Re-map
    ssd1306_send_command(0xA6); // Set Normal Display
    ssd1306_send_command(0xA8); // Set Multiplex Ratio
    ssd1306_send_command(0x3F); // 1/64 Duty
    ssd1306_send_command(0xD3); // Set Display Offset
    ssd1306_send_command(0x00); // No Offset
    ssd1306_send_command(0xD5); // Set Display Clock Divide Ratio
    ssd1306_send_command(0xF0); // Divide Ratio
    ssd1306_send_command(0xD9); // Set Pre-charge Period
    ssd1306_send_command(0x22); // Pre-charge: 2 DCLKs
    ssd1306_send_command(0xDA); // Set COM Pins Hardware Configuration
    ssd1306_send_command(0x12); // Alternative COM Pin Config
    ssd1306_send_command(0xDB); // Set VCOMH Deselect Level
    ssd1306_send_command(0x20); // VCOMH Level ~0.77 Vcc
    ssd1306_send_command(0x8D); // Enable Charge Pump
    ssd1306_send_command(0x14); // Charge Pump Enabled
    ssd1306_send_command(0xAF); // Display ON
    ssd1306_clear();
}

// Limpa o buffer do display
void ssd1306_clear() {
    memset(buffer, 0, sizeof(buffer));
}

// Desenha um caractere no display
void ssd1306_draw_char(char c, uint8_t x, uint8_t y) {
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT / 8) return;
    
    // Usa a fonte definida em font.h (formato 5x7)
    for (int i = 0; i < 5; i++) {
        uint8_t line = font[(uint8_t)c * 5 + i];
        for (int j = 0; j < 7; j++) {
            if (line & (1 << j)) {
                buffer[(y * 8 + j) * SSD1306_WIDTH + x + i] = COLOR_ON;
            }
        }
    }
}

// Atualiza o display com o buffer
void ssd1306_update() {
    for (uint8_t page = 0; page < 8; page++) {
        ssd1306_send_command(0xB0 + page); // Define a página
        ssd1306_send_command(0x00);        // Coluna baixa
        ssd1306_send_command(0x10);        // Coluna alta

        // Envia os dados do buffer para a página atual
        uint8_t buf[SSD1306_WIDTH + 1];
        buf[0] = 0x40; // Co bit (data)
        memcpy(buf + 1, &buffer[page * SSD1306_WIDTH], SSD1306_WIDTH);
        i2c_write_blocking(i2c_default, SSD1306_I2C_ADDR, buf, SSD1306_WIDTH + 1, false);
    }
}
