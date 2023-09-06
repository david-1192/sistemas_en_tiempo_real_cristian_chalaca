#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define led 4
#define boton 5

uint8_t led_level = 0;
bool boton_presionado = false; // Variable para controlar si el botón ha sido presionado
bool led_encendido = true;

esp_err_t encender_led(void);
esp_err_t alternar_led(void);
esp_err_t apagar_led(void);

void app_main(void)
{
    encender_led();
    gpio_reset_pin(boton);
    gpio_set_direction(boton, GPIO_MODE_INPUT);
    gpio_set_pull_mode(boton, GPIO_PULLUP_ONLY);

    while (1)
    {
        vTaskDelay(200 / portTICK_PERIOD_MS);

        int button_state = gpio_get_level(boton);

        boton_presionado = (button_state == 0);

        if (boton_presionado)
        {
            if (led_encendido)
            {
                apagar_led();
                led_encendido = false;
            }
            else
            {
                encender_led();
                led_encendido = true;
            }
        }
        else
        {
            if (led_encendido)
            {
                alternar_led();
            }
        }
    
        printf("LED Level: %u\n", led_level);
    }
}

esp_err_t encender_led(void)
{
    gpio_reset_pin(led);
    gpio_set_direction(led, GPIO_MODE_OUTPUT);
    return ESP_OK;
}

esp_err_t alternar_led(void)
{
    led_level = !led_level;
    gpio_set_level(led, led_level);
    return ESP_OK;
}

esp_err_t apagar_led(void)
{
    led_level = 0;
    gpio_set_level(led, led_level);
    return ESP_OK;
}