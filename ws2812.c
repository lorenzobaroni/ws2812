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

// Números para a matriz de LEDs
const uint32_t formatos_numeros[10][NUMERO_PIXELS] = {
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1}, 
    {0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0}, 
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1}, 
    {1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1}, 
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1}, 
    {1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1}, 
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1}, 
    {0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1}, 
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1}, 
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1}  
};

// Função para debouncing e alteração do número atual
void debouce_e_incrementa(uint gpio, uint32_t eventos) {
    static absolute_time_t tempo_anterior = {0};
    if (absolute_time_diff_us(tempo_anterior, get_absolute_time()) > 200000) {
        if (gpio == PINO_BOTAO_A) {
            numero_atual = (numero_atual + 1) % 10;
        } else if (gpio == PINO_BOTAO_B) {
            numero_atual = (numero_atual - 1 + 10) % 10;
        }
        tempo_anterior = get_absolute_time();
    }
}

// Função para exibir o número na matriz de LEDs
void exibir_numero(PIO pio, uint sm, const uint32_t padrao_numero[]) {
    for (int i = 0; i < NUMERO_PIXELS; i++) {
        uint32_t cor = padrao_numero[i] ? 0xFF0000 : 0x000000; 
        pio_sm_put_blocking(pio, sm, cor); 
    }
}

// Configuração dos pinos do LED RGB
void configurar_led_rgb() {
    gpio_init(PINO_RGB_R);
    gpio_init(PINO_RGB_G);
    gpio_init(PINO_RGB_B);
    gpio_set_dir(PINO_RGB_R, GPIO_OUT);
    gpio_set_dir(PINO_RGB_G, GPIO_OUT);
    gpio_set_dir(PINO_RGB_B, GPIO_OUT);
}

// Função para piscar o LED RGB
void piscar_led_rgb() {
    static absolute_time_t ultimo_piscar = {0};
    static bool estado = false;
    if (absolute_time_diff_us(ultimo_piscar, get_absolute_time()) > 100000) {
        gpio_put(PINO_RGB_R, estado); 
        estado = !estado; 
        ultimo_piscar = get_absolute_time(); 
    }
}

int main() {
    stdio_init_all();

    PIO pio = pio0;
    uint deslocamento = pio_add_program(pio, &ws2812_program);
    uint sm = pio_claim_unused_sm(pio, true);
    ws2812_program_init(pio, sm, deslocamento, PINO_WS2812, 800000, false);

    // Configura interrupções para os botões
    gpio_set_irq_enabled_with_callback(PINO_BOTAO_A, GPIO_IRQ_EDGE_FALL, true, debouce_e_incrementa);
    gpio_set_irq_enabled_with_callback(PINO_BOTAO_B, GPIO_IRQ_EDGE_FALL, true, debouce_e_incrementa);

    // Configura os LEDs RGB
    configurar_led_rgb();


    while (true) {
        piscar_led_rgb();
        exibir_numero(pio, sm, formatos_numeros[numero_atual]);
        sleep_ms(100); // Aguarda 100 ms antes de atualizar
    }
}
