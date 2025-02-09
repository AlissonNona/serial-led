#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "font.h"
#include "ws2812.h"

// Definições de pinos
#define WS2812_PIN 7
#define LED_RGB_RED_PIN 11
#define LED_RGB_GREEN_PIN 12
#define LED_RGB_BLUE_PIN 13
#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6
#define I2C_SDA_PIN 14
#define I2C_SCL_PIN 15
#define WS2812_PIN 2  // ajuste para o pino que estiver usando


// Variáveis globais
volatile bool button_a_pressed = false;
volatile bool button_b_pressed = false;
bool led_green_state = false;
bool led_blue_state = false;

// Protótipos de funções
void button_a_isr(uint gpio, uint32_t events);
void button_b_isr(uint gpio, uint32_t events);
void debounce_button(volatile bool *button_pressed, uint gpio);
void update_display(char character);
void update_ws2812_matrix(uint8_t number);
void send_serial_message(const char *message);

// Função principal
int main() {
    // Inicializações
    stdio_init_all();

    // Configuração dos LEDs RGB
    gpio_init(LED_RGB_RED_PIN);
    gpio_init(LED_RGB_GREEN_PIN);
    gpio_init(LED_RGB_BLUE_PIN);
    gpio_set_dir(LED_RGB_RED_PIN, GPIO_OUT);
    gpio_set_dir(LED_RGB_GREEN_PIN, GPIO_OUT);
    gpio_set_dir(LED_RGB_BLUE_PIN, GPIO_OUT);

    // Configuração dos botões
    gpio_init(BUTTON_A_PIN);
    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);
    gpio_pull_up(BUTTON_B_PIN);

    // Configuração das interrupções dos botões
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &button_a_isr);
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, &button_b_isr);

    // Configuração do I2C para o display SSD1306
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    // Inicialização do display SSD1306
    ssd1306_init();

    // Inicialização da WS2812
    ws2812_init();

    // Loop principal
    while (true) {
        // Verifica se o botão A foi pressionado
        if (button_a_pressed) {
            debounce_button(&button_a_pressed, BUTTON_A_PIN);
            led_green_state = !led_green_state;
            gpio_put(LED_RGB_GREEN_PIN, led_green_state);
            update_display(led_green_state ? 'G' : 'g');
            send_serial_message(led_green_state ? "LED Verde Ligado" : "LED Verde Desligado");
        }

        // Verifica se o botão B foi pressionado
        if (button_b_pressed) {
            debounce_button(&button_b_pressed, BUTTON_B_PIN);
            led_blue_state = !led_blue_state;
            gpio_put(LED_RGB_BLUE_PIN, led_blue_state);
            update_display(led_blue_state ? 'B' : 'b');
            send_serial_message(led_blue_state ? "LED Azul Ligado" : "LED Azul Desligado");
        }

        // Verifica se um caractere foi recebido via UART
        char received_char = getchar_timeout_us(1000);
        if (received_char != PICO_ERROR_TIMEOUT) {
            update_display(received_char);
            if (received_char >= '0' && received_char <= '9') {
                update_ws2812_matrix(received_char - '0');
            }
        }
    }
}

// Rotinas de interrupção
void button_a_isr(uint gpio, uint32_t events) {
    button_a_pressed = true;
}

void button_b_isr(uint gpio, uint32_t events) {
    button_b_pressed = true;
}

// Debouncing melhorado
void debounce_button(volatile bool *button_pressed, uint gpio) {
    busy_wait_ms(50); // Aguarda 50ms para evitar bouncing
    if (!gpio_get(gpio)) {
        *button_pressed = false;
    }
}

// Atualiza o display SSD1306
void update_display(char character) {
    ssd1306_clear();
    ssd1306_draw_char(0, 0, character, 1);
    ssd1306_update();
}

// Atualiza a matriz WS2812 com um número
void update_ws2812_matrix(uint8_t number) {
    // Exibe o número na matriz WS2812
    printf("Exibindo número %d na matriz WS2812\n", number);

    // Limpa a matriz
    for (int i = 0; i < 25; i++) {
        ws2812_set_pixel(i, 0, 0, 0); // Desliga todos os LEDs
    }

    // Exibe o número na matriz (exemplo simples)
    switch (number) {
        case 0:
            ws2812_set_pixel(0, 255, 0, 0); // Exemplo: LED 0 vermelho
            break;
        case 1:
            ws2812_set_pixel(1, 0, 255, 0); // Exemplo: LED 1 verde
            break;
        // Adicione mais casos para outros números
        default:
            break;
    }

    // Atualiza a matriz
    ws2812_update();
}

// Envia mensagem via UART
void send_serial_message(const char *message) {
    printf("%s\n", message); // Envia a mensagem via UART
}