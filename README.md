# Controle de Semáforo com Raspberry Pi Pico

Este projeto implementa um controle de semáforo utilizando um Raspberry Pi Pico. O sistema simula um semáforo de trânsito, alternando entre os estados vermelho, amarelo e verde em intervalos regulares.

## Componentes Utilizados

- Raspberry Pi Pico
- LEDs (Vermelho, Amarelo e Verde)
- Resistores (330Ω para cada LED)
- Jumpers
- Protoboard

## Esquema de Conexão

| LED       | GPIO do Pico |
|-----------|-------------|
| Vermelho  | 11          |
| Amarelo   | 12          |
| Verde     | 13          |

## Funcionamento

O sistema funciona com base em uma máquina de estados simples:

1. **Estado Vermelho**: O LED vermelho acende por 3 segundos.
2. **Estado Amarelo**: O LED amarelo acende por 3 segundos.
3. **Estado Verde**: O LED verde acende por 3 segundos.
4. O ciclo se repete indefinidamente.

## Código-fonte Explicado

### Definições e Configuração

```c
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
```

Incluímos as bibliotecas padrão do Raspberry Pi Pico para manipulação de GPIO e temporizadores.

### Definição dos GPIOs dos LEDs

```c
#define LED_PIN_RED    11
#define LED_PIN_YELLOW 12
#define LED_PIN_GREEN  13
```

Os pinos GPIO 11, 12 e 13 são usados para controlar os LEDs vermelho, amarelo e verde, respectivamente.

### Definição dos Estados do Semáforo

```c
typedef enum {
    RED,
    YELLOW,
    GREEN
} SemaphoreState;
```

Criamos um tipo de dado `SemaphoreState` para representar os três estados do semáforo.

### Função de Controle do Temporizador

```c
bool repeating_timer_callback(struct repeating_timer *t) {
```

Esta função é chamada a cada 3 segundos para alternar entre os estados do semáforo.

- Se o estado atual for **vermelho**, ele desliga o LED vermelho e liga o amarelo.
- Se o estado for **amarelo**, ele desliga o LED amarelo e liga o verde.
- Se o estado for **verde**, ele desliga o LED verde e liga o vermelho.

### Função `main()`

A função principal realiza a configuração dos LEDs e inicializa um temporizador periódico:

1. Configura os pinos dos LEDs como saída.
2. Define o estado inicial como **vermelho ativo**.
3. Inicia um temporizador que chama `repeating_timer_callback` a cada 3 segundos.
4. Entra em um loop infinito exibindo o estado atual do semáforo no console a cada segundo.

```c
int main() {
    stdio_init_all();
    
    gpio_init(LED_PIN_RED);
    gpio_init(LED_PIN_YELLOW);
    gpio_init(LED_PIN_GREEN);
    
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);
    gpio_set_dir(LED_PIN_YELLOW, GPIO_OUT);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);
    
    gpio_put(LED_PIN_RED, 1);
    gpio_put(LED_PIN_YELLOW, 0);
    gpio_put(LED_PIN_GREEN, 0);
    
    struct repeating_timer timer;
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);
    
    while(true) {
        printf("Estado do semaforo: %d\n", current_state);
        sleep_ms(1000);
    }
    return 0;
}
```

## Como Compilar e Executar

1. Instale o **SDK do Raspberry Pi Pico** em seu ambiente de desenvolvimento.
2. Compile o código utilizando o CMake e GCC para Pico.
3. Transfira o arquivo `.uf2` gerado para o Raspberry Pi Pico.
4. Conecte um monitor serial para visualizar os logs do sistema.

---

Projeto desenvolvido para demonstração do uso de GPIOs e temporizadores no Raspberry Pi Pico.

