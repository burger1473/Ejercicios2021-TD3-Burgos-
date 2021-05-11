#ifndef LED_H_
#define LED_H_
/*==================[ Inclusiones ]============================================*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "freertos/semphr.h" //Libreria para uso de semaforo
#include <stdint.h>

/*==================[Prototipos de funciones]======================*/
void inicializarLed( uint8_t pin );
void PrenderLed( uint8_t pin );
void ApagarLed( uint8_t pin );

#include "../main/led.c"
#endif