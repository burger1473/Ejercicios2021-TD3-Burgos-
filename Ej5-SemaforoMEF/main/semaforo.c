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

#define T 10/portTICK_PERIOD_MS //tiempo en ms de espera para volver a ingresar a la función de actualizar semaforo

// Tipo de datos para la variable de estados
typedef enum{
    ROJO,
    AMARILLO,
    VERDE
} estadoMEF;

estadoMEF estadoActual, estadoAnterior; // Variable de estado (global)

int contador; //variable para contar la cantidad de veces que ingresa a la funcipon Semaforo_siclo MEF

void Semaforo_init()
{
    configurarLed(LED_ROJO, LED_AMARILLO, LED_VERDE);
    estadoActual = ROJO;
	estadoAnterior = AMARILLO;
	contador = 0;
}

void  Semaforo_siclo()
{
    switch (estadoActual) {
		case ROJO:
        {
            // Actualizar salida del estado
			if(estadoAnterior != estadoActual)
			{
				prenderLed('R');
				apagarLed('A');
				apagarLed('V');
				estadoAnterior = estadoActual;
			}
    		// Chequear condiciones de transición de estado
			if(contador >= T_ROJO ){
				// Cambiar a otro estado
				 estadoActual = AMARILLO;
				 contador = 0;
			}
		}
		break;
		case AMARILLO:{
			// Actualizar salida del estado
			if(estadoAnterior != estadoActual)
			{
				apagarLed('R');
				prenderLed('A');
				apagarLed('V');
				// estadoAnterior = estadoActual;
			}
    		// Chequear condiciones de transición de estado
			if(contador >= T_AMARILLO ){
				// Cambiar a otro estado
				 if(estadoAnterior == ROJO)
                 { 
                     estadoActual = VERDE;  
                 }else{ 
                     if(estadoAnterior == VERDE){ 
                         estadoActual = ROJO;  
                         }
                      }
				 contador = 0;
			}
		}
		break;
		case VERDE:{
			// Actualizar salida del estado
			if(estadoAnterior != estadoActual)
			{
				apagarLed('R');
				apagarLed('A');
				prenderLed('V');
				estadoAnterior = estadoActual;
			}
    		// Chequear condiciones de transición de estado
			if(contador == T_VERDE ){
				// Cambiar a otro estado
				 estadoActual = AMARILLO;
				 contador = 0;
			}
		}
		break;
		default:{
			//Si cae en un estado no valido, reinicio
			Semaforo_init();
		}
		break;
	}
        contador++;
        vTaskDelay(T);
}