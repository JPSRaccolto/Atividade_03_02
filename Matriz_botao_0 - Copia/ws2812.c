
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"
#include "inc/font.h"
#include "inc/ssd1306.h"

#define IS_RGBW false
#define NUM_PIXELS 25
#define WS2812_PIN 7
#define tempo 400
#define LED_VERMELHO 13
#define LED_VERDE 11
#define LED_AZUL 12
#define BOTAO_A 5      
#define BOTAO_B 6      
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
volatile bool cor1 = 0;
volatile bool cor2 = 0;
int estado_atual = 0;
char valor = 0;
static volatile uint32_t ultimo_tempo = 0; // Armazena o tempo do último evento (em microssegundos)
volatile uint32_t last_irq_time_A = 0;
volatile uint32_t last_irq_time_B = 0;
absolute_time_t last_interrupt_time = 0;
int num_eventos = 10;
// Variável global para armazenar a cor (Entre 0 e 255 para intensidade)
uint8_t led_r = 0; // Intensidade do vermelho
uint8_t led_g = 0; // Intensidade do verde
uint8_t led_b = 200; // Intensidade do azul

// Buffer para armazenar quais LEDs estão ligados matriz 5x5

static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}


int numero = 10;
// Inicialização dos desenhos
int i = 0;
    // Desenho do número 0
// Matriz para armazenar os desenhos dos números de 0 a 9
double desenho0[25] = {
    0.0, 0.2, 0.2, 0.2, 0.0,
    0.0, 0.2, 0.0, 0.2, 0.0,
    0.0, 0.2, 0.0, 0.2, 0.0,
    0.0, 0.2, 0.0, 0.2, 0.0,
    0.0, 0.2, 0.2, 0.2, 0.0
};

void num0(uint8_t r, uint8_t g, uint8_t b){
    
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);  
     // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (desenho0[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
    
}

// Desenho do número 1
double desenho1[25] = 
    {0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.0, 0.2, 0.0, 0.0,
     0.0, 0.0, 0.2, 0.0, 0.0,
     0.0, 0.2, 0.2, 0.0, 0.0,
     0.0, 0.0, 0.2, 0.0, 0.0};

void num1(uint8_t r, uint8_t g, uint8_t b){
    
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);  
     // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (desenho1[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
    
}
// Desenho do número 2
double desenho2[25] = {
    0.0, 0.2, 0.2, 0.2, 0.0,
    0.0, 0.2, 0.0, 0.0, 0.0,
    0.0, 0.2, 0.2, 0.2, 0.0,
    0.0, 0.0, 0.0, 0.2, 0.0,
    0.0, 0.2, 0.2, 0.2, 0.0
};
void num2(uint8_t r, uint8_t g, uint8_t b){
    
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);  
     // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (desenho2[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
    
}

// Desenho do número 3
double desenho3[25] = {
    0.0, 0.2, 0.2, 0.2, 0.0,
    0.0, 0.0, 0.0, 0.2, 0.0,
    0.0, 0.2, 0.2, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.2, 0.0,
    0.0, 0.2, 0.2, 0.2, 0.0
};

void num3(uint8_t r, uint8_t g, uint8_t b){
    
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);  
     // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (desenho3[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
    
}
// Desenho do número 4
double desenho4[25] = {
    0.0, 0.2, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.2, 0.0,
    0.0, 0.2, 0.2, 0.2, 0.0,
    0.0, 0.2, 0.0, 0.2, 0.0,
    0.0, 0.2, 0.0, 0.2, 0.0
};
void num4(uint8_t r, uint8_t g, uint8_t b){
    
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);  
     // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (desenho4[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
    
}

// Desenho do número 5
double desenho5[25] = {
    0.0, 0.2, 0.2, 0.2, 0.0,
    0.0, 0.0, 0.0, 0.2, 0.0,
    0.0, 0.2, 0.2, 0.2, 0.0,
    0.0, 0.2, 0.0, 0.0, 0.0,
    0.0, 0.2, 0.2, 0.2, 0.0
};
void num5(uint8_t r, uint8_t g, uint8_t b){
    
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);  
     // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (desenho5[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
    
}

// Desenho do número 6
double desenho6[25] = {
    0.0, 0.2, 0.2, 0.2, 0.0,
    0.0, 0.2, 0.0, 0.2, 0.0,
    0.0, 0.2, 0.2, 0.2, 0.0,
    0.0, 0.2, 0.0, 0.0, 0.0,
    0.0, 0.2, 0.2, 0.2, 0.0
};

void num6(uint8_t r, uint8_t g, uint8_t b){
    
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);  
     // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (desenho6[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
    
}
// Desenho do número 7
double desenho7[25] = {
    0.0, 0.2, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.2, 0.0,
    0.0, 0.2, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.2, 0.0,
    0.0, 0.2, 0.2, 0.2, 0.0
};

void num7(uint8_t r, uint8_t g, uint8_t b){
    
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);  
     // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (desenho7[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
    
}
// Desenho do número 8
double desenho8[25] = {
    0.0, 0.2, 0.2, 0.2, 0.0,
    0.0, 0.2, 0.0, 0.2, 0.0,
    0.0, 0.2, 0.2, 0.2, 0.0,
    0.0, 0.2, 0.0, 0.2, 0.0,
    0.0, 0.2, 0.2, 0.2, 0.0
};
void num8(uint8_t r, uint8_t g, uint8_t b){
    
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);  
     // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (desenho8[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
    
}

// Desenho do número 9
double desenho9[25] = {
    0.0, 0.2, 0.2, 0.2, 0.0,
    0.0, 0.0, 0.0, 0.2, 0.0,
    0.0, 0.2, 0.2, 0.2, 0.0,
    0.0, 0.2, 0.0, 0.2, 0.0,
    0.0, 0.2, 0.2, 0.2, 0.0
};
void num9(uint8_t r, uint8_t g, uint8_t b){
    
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);  
     // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (desenho9[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
    
}
void atualizar_estado(char valor) {
    //if(stdio_usb_connected()){
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA); // Pull up the data line
    gpio_pull_up(I2C_SCL); // Pull up the clock line
    ssd1306_t ssd; // Inicializa a estrutura do display
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display


    switch (valor) {
        case '0':
            num0(255,0,0);
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "0", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display
            break;
        case '1':
            num1(0,255,0);
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "1", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display
            break;
        case '2':
            num2(200,100,50);
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "2", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display
            break;
        case '3':
            num3(0,0,255);
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "3", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display
            break;
        case '4':
            num4(0,75,150);
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "4", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display
            break;
        case '5':
            num5(150,90,24);
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "5", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display
            break;
        case '6':
            num6(156,36,85);
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "6", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display
            break;
        case '7':
            num7(225,100,0);
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "7", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display
            break;
        case '8':
            num8(0,201,204);
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "8", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display
            break;
        case '9':
            num9(97,0,99);
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "9", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display
            break;
        case 'A':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "A", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display
            break;
        case 'B':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "B", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display
            break;
        case 'C':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "C", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display
            break;
        case 'D':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "D", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display            
            break;
        case 'E':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "E", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display      
            break;
        case 'F':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "F", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display
            break;
        case 'G':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "G", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display   
            break;
        case 'H':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "H", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'I':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "I", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'J':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "J", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'K':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "K", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'L':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "L", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'M':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "M", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'N':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "N", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'O':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "O", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'P':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "P", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'Q':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "Q", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'R':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "R", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'S':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "S", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'T':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "T", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'U':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "U", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'V':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "V", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'W':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "W", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'X':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "X", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'Y':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "Y", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'Z':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "Z", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'a':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "a", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'b':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "b", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'c':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "c", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'd':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "d", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'e':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "e", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'f':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "f", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'g':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "g", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'h':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "h", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display '
            break;
        case 'i':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "i", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'j':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "j", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'k':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "k", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'l':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "l", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'm':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "m", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'n':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "n", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'o':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "o", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'p':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "p", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'q':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "q", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'r':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "r", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 's':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "s", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 't':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "t", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'u':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "u", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'v':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "v", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'w':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "w", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'x':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "x", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'y':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "y", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case 'z':
            printf("Recebido: '%c'\n", valor);
            ssd1306_fill(&ssd, false); // Limpa o display
            ssd1306_draw_string(&ssd, "z", 20, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(200);
            ssd1306_fill(&ssd, false); // Limpa o display 
            break;
        case '!':
            ssd1306_fill(&ssd, false);
            ssd1306_draw_string(&ssd, "LED Verde", 20, 30);
            ssd1306_draw_string(&ssd, "ligado", 18, 48);
            ssd1306_send_data(&ssd);
            break;
        case '@':
            ssd1306_fill(&ssd, false);
            ssd1306_draw_string(&ssd, "LED Verde", 20, 30);
            ssd1306_draw_string(&ssd, "desligado", 18, 48);
            ssd1306_send_data(&ssd);
            break;
        case '#':
            ssd1306_fill(&ssd, false);
            ssd1306_draw_string(&ssd, "LED Azul", 20, 30);
            ssd1306_draw_string(&ssd, "ligado", 18, 48);
            ssd1306_send_data(&ssd);
            break;
        case '$':
            ssd1306_fill(&ssd, false);
            ssd1306_draw_string(&ssd, "LED Azul", 20, 30);
            ssd1306_draw_string(&ssd, "desligado", 18, 48);
            ssd1306_send_data(&ssd);
            break;
        default:
            printf("Comando Inválido!");
            break;
            }
}
// Função de callback para interrupções
void gpio_callback(uint gpio, uint32_t events) {
    absolute_time_t now = get_absolute_time();
    int64_t diff = absolute_time_diff_us(last_interrupt_time, now);

    if (diff < 250000) return; // Debounce de 250ms = 1/4 de segundo
    last_interrupt_time = now;

    if (gpio == 5) {
        cor1 = !cor1;
        gpio_put(LED_VERDE, cor1);
        cor1 ? printf("LED Verde ligado\n") : printf("LED Verde desligado\n");  //Condição de verdadeiro e falso para print no monitor serial
        valor = cor1 ? '!' : '@'; //Condição de verdadeiro e falso para print no display
        atualizar_estado(valor); // Atualiza o display com o novo estado
    } else if (gpio == 6) {
        cor2 = !cor2;
        gpio_put(LED_AZUL, cor2);
        cor2 ? printf("LED Azul ligado\n") : printf("LED Azul desligado\n"); //Condição de verdadeiro e falso para print no monitor serial
        valor = cor2 ? '#' : '$'; //Condição de verdadeiro e falso para print no display
        atualizar_estado(valor); // Atualiza o display com o novo estado
    }
}


int main()
{
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    stdio_init_all();
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_init(LED_AZUL);
    gpio_set_dir(LED_AZUL, GPIO_OUT);
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);    
    gpio_init(BOTAO_A);
    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_A);
    gpio_pull_up(BOTAO_B);
    gpio_set_irq_enabled(5, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(6, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_callback(gpio_callback);
    irq_set_enabled(IO_IRQ_BANK0, true);
    
    //I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA); // Pull up the data line
    gpio_pull_up(I2C_SCL); // Pull up the clock line
    ssd1306_t ssd; // Inicializa a estrutura do display
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display

    //Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    
    while (1)
    {
      if (scanf("%c", &valor) == 1)
         { // Lê caractere da entrada padrão
             printf("Insira um caracter---->\n");
             atualizar_estado(valor);
             sleep_ms(20);
             ssd1306_fill(&ssd, false); // Limpa o display
    }


}
return 0;
}