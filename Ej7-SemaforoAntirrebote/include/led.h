  
#ifndef LED_H //chequea que otro archivo no haya llamado a led.h
#define LED_H

#include "driver/gpio.h"

void configurarLed(gpio_int_type_t ledR, gpio_int_type_t ledA, gpio_int_type_t ledV); //especifica en que pines conectar cada led: ROJO, AMARILLO, VERDE

void prenderLed(char led);

void apagarLed(char led);

#include  "../main/led.c"
#endif