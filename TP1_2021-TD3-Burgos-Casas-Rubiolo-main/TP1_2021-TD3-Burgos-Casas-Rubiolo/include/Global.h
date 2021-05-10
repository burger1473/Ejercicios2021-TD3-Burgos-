#ifndef Global_H
#define Global_H_

#include <stdio.h>                              // Libreria utilizada para el uso del tipo de variables uint8_t
#include <freertos\FreeRTOS.h>
#include <freertos\task.h>
#include "driver/gpio.h"

#define Cantidad_Participantes 3                // Especifico la cantidad de Participantes

extern uint8_t Salida[Cantidad_Participantes];  // Especifico pines de salida

extern uint8_t Entrada[Cantidad_Participantes]; // Especifico pines de entrada

#endif