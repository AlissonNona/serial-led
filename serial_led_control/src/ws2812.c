#include "ws2812.h"
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "ws2812.pio.h"

#define NUM_PIXELS 25 // Matriz 5x5

static uint32_t pixels[NUM_PIXELS] = {0};

// Inicializa a WS2812 via PIO
void ws2812_init() {
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, 0, offset, WS2812_PIN, 800000, false);
}

// Define a cor de um pixel (formato GRB)
void ws2812_set_pixel(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
    if (index >= NUM_PIXELS) return;
    pixels[index] = (g << 16) | (r << 8) | b; // Formato GRB
}

// Atualiza a matriz de LEDs
void ws2812_update() {
    for (int i = 0; i < NUM_PIXELS; i++) {
        pio_sm_put_blocking(pio0, 0, pixels[i]);
    }
}