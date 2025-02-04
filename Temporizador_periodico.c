#include <stdio.h>
#include "pico/stdlib.h"     // Biblioteca para funções básicas do Pico
#include "hardware/timer.h"  // Biblioteca para controle de temporizadores

#define LED_PIN_RED    11    // GPIO do LED Vermelho
#define LED_PIN_YELLOW 12    // GPIO do LED Amarelo
#define LED_PIN_GREEN  13    // GPIO do LED Verde

// Estados do semáforo (Máquina de estados)
typedef enum {
    RED,     // Estado 0: Vermelho ativo
    YELLOW,  // Estado 1: Amarelo ativo
    GREEN    // Estado 2: Verde ativo
} SemaphoreState; // Tipo de dado para o estado do semáforo

volatile SemaphoreState current_state = RED;  // Estado inicial

// Função chamada a cada 3 segundos pelo temporizador
bool repeating_timer_callback(struct repeating_timer *t) {
    switch(current_state) {
        case RED:
            // Transição Vermelho -> Amarelo
            gpio_put(LED_PIN_RED, 0);     // Desliga vermelho
            gpio_put(LED_PIN_YELLOW, 1);  // Liga amarelo 
            current_state = YELLOW;       // Atualiza estado
            break;
            
        case YELLOW:
            // Transição Amarelo -> Verde
            gpio_put(LED_PIN_YELLOW, 0);  // Desliga amarelo
            gpio_put(LED_PIN_GREEN, 1);   // Liga verde
            current_state = GREEN;        // Atualiza estado
            break;
            
        case GREEN:
            // Transição Verde -> Vermelho
            gpio_put(LED_PIN_GREEN, 0);   // Desliga verde
            gpio_put(LED_PIN_RED, 1);     // Liga vermelho
            current_state = RED;          // Atualiza estado
            break;
    }
    return true;  // Mantém o temporizador ativo
}

int main() {
    // Inicialização do sistema
    stdio_init_all();  // Habilita comunicação serial

    // Configuração dos LEDs
    gpio_init(LED_PIN_RED);
    gpio_init(LED_PIN_YELLOW);
    gpio_init(LED_PIN_GREEN);
    
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);     // Configura como saída
    gpio_set_dir(LED_PIN_YELLOW, GPIO_OUT);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);

    // Estado inicial do semáforo
    gpio_put(LED_PIN_RED, 1);     // Vermelho ligado
    gpio_put(LED_PIN_YELLOW, 0);  // Amarelo desligado
    gpio_put(LED_PIN_GREEN, 0);   // Verde desligado

    // Configura temporizador periódico
    struct repeating_timer timer;
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);

    // Loop principal
    while(true) {
        printf("Estado do semaforo: %d\n", current_state);  // Log do estado
        sleep_ms(1000);  // Espera 1 segundo entre logs
    }
    return 0;
}