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
} estadoMEF_semaforo;

estadoMEF_semaforo estadoActual_MEF_semaforo, estadoAnterior_MEF_semaforo; // Variable de estado (global)

typedef enum{
    PRENDIDO,
	APAGADO
} estadoMEF_INTERMITENTE;

estadoMEF_INTERMITENTE estadoActual_MEF_intermitente, estadoAnterior_MEF_intermitente; // Variable de estado (global)


int contador; //variable para contar la cantidad de veces que ingresa a la funcipon Semaforo_siclo MEF
int contador_intermitente;

void Semaforo_init()
{
    configurarLed(LED_ROJO, LED_AMARILLO, LED_VERDE);
    estadoActual_MEF_semaforo = ROJO;
	estadoAnterior_MEF_semaforo = AMARILLO;
	contador = 0;

	estadoActual_MEF_intermitente=PRENDIDO;
	estadoAnterior_MEF_intermitente=APAGADO;
	contador_intermitente = 0;
}

void  Semaforo_siclo()
{
    switch (estadoActual_MEF_semaforo) {
		case ROJO:
        {
            // Actualizar salida del estado
			if(estadoAnterior_MEF_semaforo != estadoActual_MEF_semaforo)
			{
				prenderLed('R');
				apagarLed('A');
				apagarLed('V');
				estadoAnterior_MEF_semaforo = estadoActual_MEF_semaforo;
			}
    		// Chequear condiciones de transición de estado
			if(contador >= T_ROJO ){
				// Cambiar a otro estado
				 estadoActual_MEF_semaforo = AMARILLO;
				 contador = 0;
			}
		}
		break;
		case AMARILLO:{
			// Actualizar salida del estado
			if(estadoAnterior_MEF_semaforo != estadoActual_MEF_semaforo)
			{
				apagarLed('R');
				prenderLed('A');
				apagarLed('V');
				// estadoAnterior_MEF_semaforo = estadoActual_MEF_semaforo;
			}
    		// Chequear condiciones de transición de estado
			if(contador >= T_AMARILLO ){
				// Cambiar a otro estado
				 if(estadoAnterior_MEF_semaforo == ROJO)
                 { 
                     estadoActual_MEF_semaforo = VERDE;  
                 }else{ 
                     if(estadoAnterior_MEF_semaforo == VERDE){ 
                         estadoActual_MEF_semaforo = ROJO;  
                         }
                      }
				 contador = 0;
			}
		}
		break;
		case VERDE:{
			// Actualizar salida del estado
			if(estadoAnterior_MEF_semaforo != estadoActual_MEF_semaforo)
			{
				apagarLed('R');
				apagarLed('A');
				prenderLed('V');
				estadoAnterior_MEF_semaforo = estadoActual_MEF_semaforo;
			}
    		// Chequear condiciones de transición de estado
			if(contador == T_VERDE ){
				// Cambiar a otro estado
				 estadoActual_MEF_semaforo = AMARILLO;
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

void  Semaforo_intermitente()
{
	switch (estadoActual_MEF_intermitente) {
		
		case PRENDIDO:{
			// Actualizar salida del estado
			
			if(estadoAnterior_MEF_intermitente != estadoActual_MEF_intermitente)
			{
				apagarLed('R');
				prenderLed('A');
				apagarLed('V');
				estadoAnterior_MEF_intermitente = estadoActual_MEF_intermitente;
			}
    		// Chequear condiciones de transición de estado
			if(contador_intermitente >= T_AMARILLO ){
				// Cambiar a otro estado
				 estadoActual_MEF_intermitente = APAGADO;	
				 contador_intermitente = 0;
			}
		}
		break;
		case APAGADO:{
			// Actualizar salida del estado
			
			if(estadoAnterior_MEF_intermitente != estadoActual_MEF_intermitente)
			{
				apagarLed('R');
				apagarLed('A');
				apagarLed('V');
				estadoAnterior_MEF_intermitente = estadoActual_MEF_intermitente;
			}
    		// Chequear condiciones de transición de estado
			if(contador_intermitente >= T_AMARILLO ){
				// Cambiar a otro estado
				 estadoActual_MEF_intermitente = PRENDIDO;
				 contador_intermitente = 0;
			}
		}
		break;
		default:{
			//Si cae en un estado no valido, reinicio
			Semaforo_init();			
		}
		break;
	}
        contador_intermitente++;
        vTaskDelay(T);

}