#ifndef LED_H_
#define LED_H_
/*==================[ Inclusiones ]============================================*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

/*==================[ Definiciones ]===================================*/
#define TIEMPO_NO_VALIDO   0

/*==================[Prototipos de funciones]======================*/
void crear_TareaLED(void);

/*==================[Variables]======================*/
extern TickType_t dif;

#include "../main/led.c"
#endif