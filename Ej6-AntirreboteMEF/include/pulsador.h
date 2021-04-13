#ifndef PULSADOR_H
#define PULSADOR_H

void Pulsador_init(int8_t PULSADOR);   //configura el pin como entrada
void  Actualizar_estado(int8_t PULSADOR); //realizo el cambio de los leds en un siclo del semaforo

#include  "../main/pulsador.c"
#endif