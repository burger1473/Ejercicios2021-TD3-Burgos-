#include "../include/led.h"
#include "../include/semaforo.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define LED_ROJO 4
#define LED_AMARILLO 2
#define LED_VERDE 14

#define T_ROJO		500
#define T_AMARILLO	100
#define T_VERDE		300


void iniciarSemaforo()
{
    configurarLed(LED_ROJO, LED_AMARILLO, LED_VERDE);
}

void actualizarSemaforo()
{
    prenderLed('Rojo');
    vTaskDelay(T_ROJO);
    apagarLed('Rojo');
    prenderLed('Amarillo');
    vTaskDelay(T_AMARILLO);
    apagarLed('Amarillo');
    prenderLed('Verde');
    vTaskDelay(T_VERDE);
    apagarLed('Verde');
    prenderLed('Amarillo');
    vTaskDelay(T_AMARILLO);
    apagarLed('Amarillo');
}