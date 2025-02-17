#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "ssd1306.h"
#include <math.h>

// Definições de pinos e constantes
#define BOTAO_A 5
#define LED_B 12
#define LED_G 11
#define LED_R 13
#define PWM_WRAP 4095
#define I2C_PORT i2c1
#define I2C_SDA 15
#define I2C_SCL 14
#define ENDERECO 0x3C
#define WIDTH 128  // Largura da tela
#define HEIGHT 64  // Altura da tela
#define QUADRADO_TAM 8 // Tamanho do quadrado
#define EIXO_X 26       // Pino de leitura do eixo X do joystick
#define EIXO_Y 27       // Pino de leitura do eixo Y do joystick
#define SELEC 22        // Pino de leitura do botão do joystick
#define ADC_CHANNEL_X 1 // Canal ADC para o eixo X
#define ADC_CHANNEL_Y 0 // Canal ADC para o eixo Y

// Variáveis globais
bool cor = false;
bool cor1 = false;
absolute_time_t last_interrupt_time = 0;
ssd1306_t ssd;
int estilo_borda = 0; // Variável para controlar o estilo da borda

// Valores calibrados do joystick 
const int x_min = 0;
const int x_max = 4095;
const int y_min = 0;
const int y_max = 4095;

// Protótipos de funções
void setup_joystick_btns();
void leitura_joystick(uint16_t *valor_X, uint16_t *valor_Y);
void gpio_callback(uint gpio, uint32_t events);
void pwm_init_gpio(uint gpio, uint wrap);
int mapear_adc_para_tela(int adc_valor, int tela_tamanho, int offset, int joystick_min, int joystick_max);
void desenhar_borda(int estilo_borda);
void inicializar_display();
void atualizar_leds(uint16_t valor_X, uint16_t valor_Y);

// Função para inicializar o display
void inicializar_display() {
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ENDERECO, I2C_PORT);
    ssd1306_config(&ssd);
}

// Função para configurar o joystick e botões
void setup_joystick_btns() {
    adc_init();
    adc_gpio_init(EIXO_X);
    adc_gpio_init(EIXO_Y);
    gpio_init(SELEC);
    gpio_set_dir(SELEC, GPIO_IN);
    gpio_pull_up(SELEC);
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);
}

// Função para ler os valores do joystick
void leitura_joystick(uint16_t *valor_X, uint16_t *valor_Y) {
    adc_select_input(ADC_CHANNEL_X);
    sleep_us(2);
    *valor_X = adc_read();
    adc_select_input(ADC_CHANNEL_Y);
    sleep_us(2);
    *valor_Y = adc_read();
}

// Função para mapear valores do ADC para a tela
int mapear_adc_para_tela(int adc_valor, int tela_tamanho, int offset, int joystick_min, int joystick_max) {
    float proporcao = (float)(adc_valor - joystick_min) / (joystick_max - joystick_min);
    return (int)(proporcao * (tela_tamanho - offset));
}

// Função para desenhar a borda com diferentes estilos
void desenhar_borda(int estilo_borda) {
    switch (estilo_borda) {
        case 0: // Borda sólida
            ssd1306_rect(&ssd, 0, 0, WIDTH, HEIGHT, true, false);
            break;

        case 1: // Borda tracejada
            for (int i = 0; i < WIDTH; i += 4) {
                ssd1306_line(&ssd, i, 0, i + 2, 0, true); // Linha superior
                ssd1306_line(&ssd, i, HEIGHT - 1, i + 2, HEIGHT - 1, true); // Linha inferior
            }
            for (int i = 0; i < HEIGHT; i += 4) {
                ssd1306_line(&ssd, 0, i, 0, i + 2, true); // Linha esquerda
                ssd1306_line(&ssd, WIDTH - 1, i, WIDTH - 1, i + 2, true); // Linha direita
            }
            break;

        case 2: // Borda pontilhada
            for (int i = 0; i < WIDTH; i += 2) {
                ssd1306_pixel(&ssd, i, 0, true); // Pontos na linha superior
                ssd1306_pixel(&ssd, i, HEIGHT - 1, true); // Pontos na linha inferior
            }
            for (int i = 0; i < HEIGHT; i += 2) {
                ssd1306_pixel(&ssd, 0, i, true); // Pontos na linha esquerda
                ssd1306_pixel(&ssd, WIDTH - 1, i, true); // Pontos na linha direita
            }
            break;

        case 3: // Borda dupla
            ssd1306_rect(&ssd, 0, 0, WIDTH, HEIGHT, true, false); // Borda externa
            ssd1306_rect(&ssd, 2, 2, WIDTH - 4, HEIGHT - 4, true, false); // Borda interna
            break;

        case 4: // Borda com cantos arredondados
            ssd1306_line(&ssd, 4, 0, WIDTH - 5, 0, true); // Linha superior
            ssd1306_line(&ssd, 4, HEIGHT - 1, WIDTH - 5, HEIGHT - 1, true); // Linha inferior
            ssd1306_line(&ssd, 0, 4, 0, HEIGHT - 5, true); // Linha esquerda
            ssd1306_line(&ssd, WIDTH - 1, 4, WIDTH - 1, HEIGHT - 5, true); // Linha direita

            // Cantos arredondados
            ssd1306_pixel(&ssd, 2, 1, true);
            ssd1306_pixel(&ssd, 1, 2, true);
            ssd1306_pixel(&ssd, WIDTH - 3, 1, true);
            ssd1306_pixel(&ssd, WIDTH - 2, 2, true);
            ssd1306_pixel(&ssd, 2, HEIGHT - 2, true);
            ssd1306_pixel(&ssd, 1, HEIGHT - 3, true);
            ssd1306_pixel(&ssd, WIDTH - 3, HEIGHT - 2, true);
            ssd1306_pixel(&ssd, WIDTH - 2, HEIGHT - 3, true);
            break;

        default: // Sem borda
            break;
    }
}

// Função para atualizar os LEDs
void atualizar_leds(uint16_t valor_X, uint16_t valor_Y) {
    pwm_set_gpio_level(LED_B, cor1 ? abs(valor_Y - 2047) : 0);
    pwm_set_gpio_level(LED_R, cor1 ? abs(valor_X - 2047) : 0);
}

// Callback para os botões
void gpio_callback(uint gpio, uint32_t events) {
    absolute_time_t now = get_absolute_time();
    int64_t diff = absolute_time_diff_us(last_interrupt_time, now);

    if (diff < 250000) return;
    last_interrupt_time = now;

    if (gpio == SELEC) {
        cor = !cor;
        gpio_put(LED_G, cor);
        estilo_borda = (estilo_borda + 1) % 5; // Alterna entre os estilos de borda
    }
    if (gpio == BOTAO_A) {
        cor1 = !cor1;
        pwm_set_gpio_level(LED_B, cor1 ? 4095 : 0);
        pwm_set_gpio_level(LED_R, cor1 ? 4095 : 0);
    }
}

// Implementação da função pwm_init_gpio
void pwm_init_gpio(uint gpio, uint wrap) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_wrap(&config, wrap);
    pwm_init(slice_num, &config, true);
}
