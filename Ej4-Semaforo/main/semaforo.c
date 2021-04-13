#include "../include/led.h"
#include "../include/semaforo.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define LED_ROJO 4
#define LED_AMARILLO 2
#define LED_VERDE 14
#define PULSADOR 16

#define T_ROJO		500
#define T_AMARILLO	100
#define T_VERDE		300


void Semaforo_init()
{
    configurarLed(LED_ROJO, LED_AMARILLO, LED_VERDE);
    gpio_pad_select_gpio(PULSADOR);
	gpio_set_direction(PULSADOR, GPIO_MODE_INPUT);
	gpio_set_pull_mode(PULSADOR, GPIO_PULLUP_ONLY);
}

void  Semaforo_siclo()
{
    if(gpio_get_level(PULSADOR) == 1){
        prenderLed('R');
        vTaskDelay(T_ROJO);
        apagarLed('R');
        prenderLed('A');
        vTaskDelay(T_AMARILLO);
        apagarLed('A');
        prenderLed('V');
        vTaskDelay(T_VERDE);
        apagarLed('V');
        prenderLed('A');
        vTaskDelay(T_AMARILLO);
        apagarLed('A');
    }else{
        apagarLed('V');
        apagarLed('R');
        prenderLed('A');
        vTaskDelay(T_AMARILLO);
        apagarLed('A');
        vTaskDelay(T_AMARILLO);

    }
}