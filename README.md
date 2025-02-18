# Projeto BitDogLab: Controle de LEDs e Display com Joystick

Este projeto foi desenvolvido como parte da Unidade 4, Capítulo 8, com o objetivo de consolidar os conceitos sobre o uso de conversores analógico-digitais (ADC), PWM, I2C e interrupções no microcontrolador RP2040, utilizando a placa de desenvolvimento BitDogLab.

## Descrição do Projeto

O projeto utiliza um joystick para controlar a intensidade de LEDs RGB e a posição de um quadrado em um display OLED. As funcionalidades incluem:

- **Controle de LEDs**:
  - O eixo Y do joystick controla a intensidade do LED Azul.
  - O eixo X controla a intensidade do LED Vermelho.
  - O LED Verde é alternado pelo botão do joystick.

- **Display OLED**:
  - Um quadrado de 8x8 pixels se move no display conforme a posição do joystick.
  - O botão do joystick alterna entre diferentes estilos de borda no display.

- **Botão A**:
  - Ativa ou desativa o controle dos LEDs Azul e Vermelho.

## Componentes Utilizados

- **Joystick**:
  - Eixo X: GPIO 26 (ADC0)
  - Eixo Y: GPIO 27 (ADC1)
  - Botão: GPIO 22

- **LEDs RGB**:
  - Verde: GPIO 11
  - Azul: GPIO 12
  - Vermelho: GPIO 13

- **Botão A**: GPIO 5

- **Display OLED SSD1306**:
  - Interface I2C (GPIO 14 - SCL, GPIO 15 - SDA)

## Funcionalidades Implementadas

1. **Leitura do Joystick**:
   - Os valores analógicos dos eixos X e Y são convertidos pelo ADC e mapeados para controlar a posição do quadrado no display e a intensidade dos LEDs.

2. **Controle de LEDs via PWM**:
   - A intensidade dos LEDs Azul e Vermelho é ajustada conforme a posição do joystick.
   - O LED Verde é alternado pelo botão do joystick.

3. **Display OLED**:
   - O quadrado se move proporcionalmente aos valores do joystick.
   - O botão do joystick alterna entre 5 estilos de borda:
     - Sólida
     - Tracejada
     - Pontilhada
     - Dupla
     - Cantos arredondados

4. **Interrupções e Debouncing**:
   - Os botões utilizam interrupções para garantir respostas rápidas.
   - O debouncing foi implementado via software para evitar leituras múltiplas.

## Estrutura do Código

O código está organizado em funções para facilitar a leitura e manutenção:

- **Funções Principais**:
  - `main`: Loop principal do programa.
  - `inicializar_display`: Configura o display OLED.
  - `setup_joystick_btns`: Configura o joystick e os botões.
  - `leitura_joystick`: Lê os valores dos eixos X e Y.
  - `desenhar_borda`: Desenha a borda no display conforme o estilo selecionado.
  - `atualizar_leds`: Atualiza a intensidade dos LEDs Azul e Vermelho.

- **Interrupções**:
  - `gpio_callback`: Gerencia as ações dos botões (SELEC e BOTAO_A).

## Como Executar o Projeto

1. **Requisitos**:
   - Placa BitDogLab com RP2040.
   - Display OLED SSD1306.
   - Joystick e LEDs RGB conectados conforme descrito.

2. **Compilação**:
   - Utilize o ambiente de desenvolvimento Pico SDK para compilar o código.
   - Carregue o binário gerado na placa BitDogLab.

3. **Execução**:
     - Clone o repositório em sua máquina local:
       ```bash
       git clone https://github.com/Luizgvv/ADC.git
   - Conecte a placa BitDogLab ao computador.
   - Abra o código no VS Code e compile o projeto.
   - O projeto começa automaticamente após o carregamento.
   - Utilize o joystick e os botões para interagir com o sistema.
   - Observe as funcionalidades descritas no projeto em execução.
   
### Vídeo de Demonstração

[![Demonstração na Placa](https://img.youtube.com/vi/lhfL9c6oTFo/0.jpg)](https://youtu.be/lhfL9c6oTFo) .

Um vídeo de demonstração com a explicação e exibição do funcionamento do projeto foi produzido. Clique na imagem acima ou no link abaixo para assistir:

🔗 [Assistir no YouTube](https://youtu.be/lhfL9c6oTFo)  .




