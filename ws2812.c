#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/irq.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/sync.h"
#include "ws2812.pio.h" // Biblioteca para controlar LEDs WS2812

// Define constantes
#define NUMERO_PIXELS 25           
#define PINO_WS2812 7              
#define PINO_RGB_G 11              
#define PINO_RGB_B 12              
#define PINO_RGB_R 13              
#define PINO_BOTAO_A 5             
#define PINO_BOTAO_B 6             

// Variável global para rastrear o número atual a ser exibido
volatile int numero_atual = 0;

// Função principal
int main() {
    // Inicializa entrada e saída padrão
    stdio_init_all();

    // Inicializa o PIO e configura o programa para LEDs WS2812
    PIO pio = pio0;
    uint deslocamento = pio_add_program(pio, &ws2812_program);
    uint sm = pio_claim_unused_sm(pio, true);
    ws2812_program_init(pio, sm, deslocamento, PINO_WS2812, 800000, false);

    // Configura interrupções para os botões
    gpio_set_irq_enabled_with_callback(PINO_BOTAO_A, GPIO_IRQ_EDGE_FALL, true, );
    gpio_set_irq_enabled_with_callback(PINO_BOTAO_B, GPIO_IRQ_EDGE_FALL, true, );

    // Configura os LEDs RGB
    configurar_led_rgb();

    // Loop principal
    while (true) {
        sleep_ms(100); // Aguarda 100 ms antes de atualizar
    }
}
