#ifndef WS2812_H
#define WS2812_H

#include <stdint.h>
#include "hardware/pio.h"

// Se necessário, defina o pino para os WS2812.
// Se PICO_DEFAULT_WS2812_PIN não estiver definido, defina WS2812_PIN para o pino que você estiver utilizando.
#ifdef PICO_DEFAULT_WS2812_PIN
    #define WS2812_PIN PICO_DEFAULT_WS2812_PIN
#else
#define WS2812_PIN 7 // Use o pino GPIO 7// ajuste para o pino que estiver usando
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Inicializa os LEDs WS2812 via PIO
void ws2812_init(void);

// Define a cor de um pixel (formato GRB)
void ws2812_set_pixel(uint8_t index, uint8_t r, uint8_t g, uint8_t b);

// Envia os dados para atualizar a matriz de LEDs
void ws2812_update(void);

#ifdef __cplusplus
}
#endif

#endif // WS2812_H
