#include "funcoes.h"

// Função principal
int main() {
    stdio_init_all();

    // Inicializa PWM e GPIOs
    pwm_init_gpio(LED_B, PWM_WRAP);
    pwm_init_gpio(LED_R, PWM_WRAP);
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_put(LED_G, 0);

    // Inicializa botões
    gpio_init(SELEC);
    gpio_set_dir(SELEC, GPIO_IN);
    gpio_pull_up(SELEC);
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);
    gpio_set_irq_enabled_with_callback(SELEC, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    // Inicializa display e joystick
    inicializar_display();
    setup_joystick_btns();

    uint16_t valor_X, valor_Y;

    while (true) {
        // Lê os valores do joystick
        leitura_joystick(&valor_X, &valor_Y);

        // Mapeia os valores do joystick para a tela
        int pos_X = mapear_adc_para_tela(valor_X, WIDTH, QUADRADO_TAM, x_min, x_max);
        int pos_Y = mapear_adc_para_tela(valor_Y, HEIGHT, QUADRADO_TAM, y_max, y_min);

        // Limita os valores para evitar que o quadrado saia da tela
        pos_X = fmin(fmax(pos_X, 0), WIDTH - QUADRADO_TAM);
        pos_Y = fmin(fmax(pos_Y, 0), HEIGHT - QUADRADO_TAM);

        // Atualiza o display
        ssd1306_fill(&ssd, false);
        ssd1306_rect(&ssd, pos_Y, pos_X, QUADRADO_TAM, QUADRADO_TAM, true, false);
        desenhar_borda(estilo_borda); // Desenha a borda com o estilo atual
        ssd1306_send_data(&ssd);

        // Atualiza os LEDs
        atualizar_leds(valor_X, valor_Y);

        sleep_ms(100);
    }
}

