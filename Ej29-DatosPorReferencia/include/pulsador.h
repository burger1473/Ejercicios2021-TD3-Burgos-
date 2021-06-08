#ifndef PULSADOR_H_
#define PULSADOR_H_
/*==================[ Inclusiones ]============================================*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <stdint.h>
#include "freertos/queue.h" //incluyo la libreria para usar cola

/*==================[ Variables ]============================================*/


/*==================[ Definiciones ]===================================*/
#define TIEMPO_NO_VALIDO   0

typedef enum
{
    ALTO,
    BAJO,
    DESCENDENTE,
    ASCENDENTE
} estadoPulsador;

typedef struct
{
    gpio_int_type_t tecla;
    estadoPulsador estado;   //variables
    TickType_t tiempoBajo;		//tiempo de la última transición del estado alto a bajo
    TickType_t tiempoAlto;		    //tiempo de la última transición del estado bajo a alto
    TickType_t diferenciaTiempo;	    //variables
    QueueHandle_t cola;
} pulsadorInfo;

typedef struct
{
    uint8_t indice_Led;
    TickType_t tiempo;  
} estructura_t;

/*==================[Prototipos de funciones]======================*/
void inicializarPulsador( void);
TickType_t obtenerDiferencia( uint8_t  i);
void borrarDiferencia( uint8_t  i);

#include "../main/pulsador.c"
#endif