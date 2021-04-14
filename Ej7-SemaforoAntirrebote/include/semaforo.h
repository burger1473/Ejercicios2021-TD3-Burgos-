#ifndef SEMAFORO_H
#define SEMAFORO_H

void Semaforo_init();   //configura los pines de los leds
void Semaforo_siclo(); //realizo el cambio de los leds en un siclo del semaforo
void Semaforo_intermitente();//realizo cambio de led amarillo prendido a apagado

#include  "../main/semaforo.c"
#endif