#include "ssd1306.h"
#include <stdlib.h>

// Inicializa o display
void ssd1306_init(ssd1306_t *ssd, i2c_inst_t *i2c_port) {
    ssd->width = SSD1306_WIDTH;
    ssd->height = SSD1306_HEIGHT;
    ssd->pages = SSD1306_HEIGHT / 8;
    ssd->external_vcc = false;
    ssd->i2c_port = i2c_port;
    
    // Aloca o buffer (128x64 pixels / 8 bits por byte)
    ssd->buffer = (uint8_t*)malloc(SSD1306_WIDTH * ssd->pages);
    memset(ssd->buffer, 0, SSD1306_WIDTH * ssd->pages);

    // Sequência de inicialização
    uint8_t init_sequence[] = {
        SSD1306_SET_DISPLAY_OFF,
        SSD1306_SET_MEMORY_MODE, 0x00, // Modo horizontal
        SSD1306_SET_DISPLAY_START,
        SSD1306_SET_SEG_REMAP | 0x01,  // Flip horizontal
        SSD1306_SET_MUX_RATIO, 0x3F,
        SSD1306_SET_COM_SCAN_DIR | 0x08, // Flip vertical
        SSD1306_SET_DISPLAY_OFFSET, 0x00,
        SSD1306_SET_COM_PINS, 0x12,
        SSD1306_SET_CONTRAST, 0x7F,
        SSD1306_SET_PRECHARGE, 0xF1,
        SSD1306_SET_VCOM_DESELECT, 0x40,
        SSD1306_SET_CHARGE_PUMP, 0x14, // Habilita charge pump
        SSD1306_SET_DISPLAY_ON
    };

    // Envia os comandos via I2C
    for (size_t i = 0; i < sizeof(init_sequence); i++) {
        uint8_t cmd[] = {0x00, init_sequence[i]}; // 0x00 = Co bit (command)
        i2c_write_blocking(ssd->i2c_port, SSD1306_ADDRESS, cmd, 2, false);
    }
}

// Limpa o buffer
void ssd1306_clear(ssd1306_t *ssd) {
    memset(ssd->buffer, 0, SSD1306_WIDTH * ssd->pages);
}

// Atualiza o display
void ssd1306_update(ssd1306_t *ssd) {
    for (uint8_t page = 0; page < ssd->pages; page++) {
        uint8_t cmd[] = {
            0x00, // Co bit (command)
            SSD1306_SET_PAGE_ADDR, page, page, // Define a página
            SSD1306_SET_COL_ADDR, 0, SSD1306_WIDTH - 1 // Define a coluna
        };
        i2c_write_blocking(ssd->i2c_port, SSD1306_ADDRESS, cmd, sizeof(cmd), false);
        
        // Envia os dados da página
        uint8_t data[SSD1306_WIDTH + 1];
        data[0] = 0x40; // Co bit (data)
        memcpy(data + 1, &ssd->buffer[page * SSD1306_WIDTH], SSD1306_WIDTH);
        i2c_write_blocking(ssd->i2c_port, SSD1306_ADDRESS, data, sizeof(data), false);
    }
}