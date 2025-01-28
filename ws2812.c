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
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1}, // 0
    {0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0}, // 1
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1}, // 2 
    {1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1}, // 3
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1}, // 4
    {1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1}, // 5
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1}, // 6
    {0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1}, // 7
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1}, // 8
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1}  // 9
};

// Função para debouncing e alteração do número atual
void debouce_e_incrementa(uint gpio, uint32_t eventos) {
    static absolute_time_t tempo_anterior = {0};
    // Verifica se passou tempo suficiente para evitar múltiplos acionamentos
    if (absolute_time_diff_us(tempo_anterior, get_absolute_time()) > 200000) {
        if (gpio == PINO_BOTAO_A) {
            numero_atual = (numero_atual + 1) % 10; // Incrementa e volta para 0 se passar de 9
        } else if (gpio == PINO_BOTAO_B) {
            numero_atual = (numero_atual - 1 + 10) % 10; // Decrementa e volta para 9 se for menor que 0
        }
        tempo_anterior = get_absolute_time(); // Atualiza o tempo da última ação
    }
}

// Função para exibir o número na matriz de LEDs
void exibir_numero(PIO pio, uint sm, const uint32_t padrao_numero[]) {
    for (int i = 0; i < NUMERO_PIXELS; i++) {
        uint32_t cor = padrao_numero[i] ? 0xFF0000 : 0x000000; // Define cor: vermelho ou apagado
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

// Configuração inicial dos botões
void configurar_botoes() {
    gpio_init(PINO_BOTAO_A);
    gpio_init(PINO_BOTAO_B);
    gpio_set_dir(PINO_BOTAO_A, GPIO_IN);
    gpio_set_dir(PINO_BOTAO_B, GPIO_IN);
    gpio_pull_up(PINO_BOTAO_A); // Ativa o pull-up interno
    gpio_pull_up(PINO_BOTAO_B); // Ativa o pull-up interno
}

// Função para piscar o LED RGB
void piscar_led_rgb() {
    static absolute_time_t ultimo_piscar = {0};
    static bool estado = false;
    if (absolute_time_diff_us(ultimo_piscar, get_absolute_time()) > 100000) {
        gpio_put(PINO_RGB_R, estado); // Alterna o estado do LED vermelho
        estado = !estado; // Inverte o estado
        ultimo_piscar = get_absolute_time(); // Atualiza o tempo do último piscar
    }
}

int main() {

    stdio_init_all();

    // Inicializa o PIO e configura o programa para LEDs WS2812
    PIO pio = pio0;
    uint deslocamento = pio_add_program(pio, &ws2812_program);
    uint sm = pio_claim_unused_sm(pio, true);
    ws2812_program_init(pio, sm, deslocamento, PINO_WS2812, 800000, false);

    configurar_botoes();

    // Configura interrupções para os botões
    gpio_set_irq_enabled_with_callback(PINO_BOTAO_A, GPIO_IRQ_EDGE_FALL, true, debouce_e_incrementa);
    gpio_set_irq_enabled_with_callback(PINO_BOTAO_B, GPIO_IRQ_EDGE_FALL, true, debouce_e_incrementa);

    configurar_led_rgb();

    while (true) {
        piscar_led_rgb();
        exibir_numero(pio, sm, formatos_numeros[numero_atual]);
        sleep_ms(100);
    }
}