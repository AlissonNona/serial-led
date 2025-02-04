#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

// Definição dos GPIOs para os LEDs
#define LED_RED 11
#define LED_YELLOW 12
#define LED_GREEN 13

// Enumeração para os estados do semáforo
typedef enum {
    RED,
    YELLOW,
    GREEN
} TrafficLightState;

TrafficLightState currentState = RED; // Estado inicial do semáforo

// Função de callback para o temporizador periódico
bool repeating_timer_callback(struct repeating_timer *t) {
    switch (currentState) {
        case RED:
            printf("Mudando para AMARELO\n");
            gpio_put(LED_RED, 0);    // Desliga o LED vermelho
            gpio_put(LED_YELLOW, 1); // Liga o LED amarelo
            currentState = YELLOW;   // Muda para o estado amarelo
            break;
        case YELLOW:
            printf("Mudando para VERDE\n");
            gpio_put(LED_YELLOW, 0); // Desliga o LED amarelo
            gpio_put(LED_GREEN, 1);  // Liga o LED verde
            currentState = GREEN;    // Muda para o estado verde
            break;
        case GREEN:
            printf("Mudando para VERMELHO\n");
            gpio_put(LED_GREEN, 0);  // Desliga o LED verde
            gpio_put(LED_RED, 1);    // Liga o LED vermelho
            currentState = RED;      // Muda para o estado vermelho
            break;
    }
    return true; // Mantém o temporizador ativo
}

int main() {
    stdio_init_all(); // Inicializa a comunicação serial (USB)
    
    // Inicializa os GPIOs dos LEDs
    gpio_init(LED_RED);
    gpio_init(LED_YELLOW);
    gpio_init(LED_GREEN);

    // Configura os GPIOs como saída
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_YELLOW, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);

    // Define o estado inicial dos LEDs
    gpio_put(LED_RED, 1);   // Começa com o LED vermelho ligado
    gpio_put(LED_YELLOW, 0); // LED amarelo desligado
    gpio_put(LED_GREEN, 0);  // LED verde desligado

    // Configura o temporizador periódico (3 segundos)
    struct repeating_timer timer;
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);

    // Loop principal
    while (true) {
        sleep_ms(1000); // Espera 1 segundo
    }

    return 0;
}
