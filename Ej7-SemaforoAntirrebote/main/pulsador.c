#include "../include/pulsador.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define T_ESPERA 5 //cantidad de veces que va a ingresar antes de cambiar de estado

// Tipo de datos para la variable de estados
typedef enum{
    BAJO,
    ALTO,
    CRECIENTE,
    DECRECIENTE
} estadoMEF_pulsador;

estadoMEF_pulsador estadoActual_MEF_pulsador, estadoAnterior_MEF_pulsador; // Variable de estado (global)



void Pulsador_init(int8_t PULSADOR)
{
   estadoActual_MEF_pulsador = BAJO;  // Se configura estado inicial
   gpio_pad_select_gpio(PULSADOR);
	gpio_set_direction(PULSADOR, GPIO_MODE_INPUT);
	gpio_set_pull_mode(PULSADOR, GPIO_PULLUP_ONLY);
}

void Actualizar_estado(int8_t PULSADOR)
{
   static uint8_t contDescendente = 0;
   static uint8_t contAscendente = 0;

   switch(estadoActual_MEF_pulsador){

      case BAJO: 
         if( gpio_get_level(PULSADOR) ){
            estadoActual_MEF_pulsador = CRECIENTE; //pasa a estado ascendente
         }
      break;

      case CRECIENTE:      
         if( contAscendente >= T_ESPERA ){ //contador para esperar un cierto tiempo antes de verificar si esta en alto
            if( gpio_get_level(PULSADOR) ){ //si luego del tiempo, el boton sigue en alto
               estadoActual_MEF_pulsador = ALTO; //se cambia el estado a alto
               Modo=1; //se indica que el boton esta precionado y se pone en modo intermitente
               printf("CAMBIO A MODO 1\n");
            } else{ //si luego del tiempo, el boton esta en estado bajo, fue un rebote o ruido
               estadoActual_MEF_pulsador = BAJO; //se mantiene en estado bajo
            }
            contAscendente = 0; //reseteo contador
         }
         contAscendente++; //incremento
      break;

      case ALTO:
         if( !gpio_get_level(PULSADOR) ){ //si se esta en estado alto y el pulsador pasa a estado 0
            estadoActual_MEF_pulsador = DECRECIENTE; //se pasa al estado desendente
         }
      break;

      case DECRECIENTE:      
         if( contDescendente >= T_ESPERA ){ //contador para esperar un cierto tiempo y controlar que permanece en bajo
            if( !gpio_get_level(PULSADOR) ){ //si leugo del tiempo, permanece en bajo el pulsador
               estadoActual_MEF_pulsador = BAJO; //se cambia a estado bajo
               Modo=0; //se indica que el boton esta en bajo y se pone en modo normal
               printf("CAMBIO A MODO 0\n");
            } else{ //si luego del tiempo, el pulsador permanece en alto, fue un rebote o un ruido
               estadoActual_MEF_pulsador = ALTO; //por lo tanto se permanece en estado alto
            }
            contDescendente = 0; //reseteo contador
         }
         contDescendente++; //incremento
      break;

      default:
         estadoActual_MEF_pulsador = ALTO; 
      break;
   }

   
}

