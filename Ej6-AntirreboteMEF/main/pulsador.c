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
} estadoMEF;

estadoMEF estadoActual, estadoAnterior; // Variable de estado (global)

void botonPresionado( void )
{
    printf("Boton presionado\n");
}

void botonLiberado( void )
{
    printf("Boton liberado\n");
}


void Pulsador_init(int8_t PULSADOR)
{
    estadoActual = BAJO;  // Se configura estado inicial
    gpio_pad_select_gpio(PULSADOR);
	gpio_set_direction(PULSADOR, GPIO_MODE_INPUT);
	gpio_set_pull_mode(PULSADOR, GPIO_PULLUP_ONLY);
}

void Actualizar_estado(int8_t PULSADOR)
{
   static uint8_t contDescendente = 0;
   static uint8_t contAscendente = 0;

   switch(estadoActual){

      case BAJO: 
         if( gpio_get_level(PULSADOR) ){
            estadoActual = CRECIENTE; //pasa a estado ascendente
         }
      break;

      case CRECIENTE:      
         if( contAscendente >= T_ESPERA ){ //contador para esperar un cierto tiempo antes de verificar si esta en alto
            if( gpio_get_level(PULSADOR) ){ //si luego del tiempo, el boton sigue en alto
               estadoActual = ALTO; //se cambia el estado a alto
               botonPresionado(); //se indica que el boton esta precionado 
            } else{ //si luego del tiempo, el boton esta en estado bajo, fue un rebote o ruido
               estadoActual = BAJO; //se mantiene en estado bajo
            }
            contAscendente = 0; //reseteo contador
         }
         contAscendente++; //incremento
      break;

      case ALTO:
         if( !gpio_get_level(PULSADOR) ){ //si se esta en estado alto y el pulsador pasa a estado 0
            estadoActual = DECRECIENTE; //se pasa al estado desendente
         }
      break;

      case DECRECIENTE:      
         if( contDescendente >= T_ESPERA ){ //contador para esperar un cierto tiempo y controlar que permanece en bajo
            if( !gpio_get_level(PULSADOR) ){ //si leugo del tiempo, permanece en bajo el pulsador
               estadoActual = BAJO; //se cambia a estado bajo
               botonLiberado();//se indica que el boton esta en estado bajo
            } else{ //si luego del tiempo, el pulsador permanece en alto, fue un rebote o un ruido
               estadoActual = ALTO; //por lo tanto se permanece en estado alto
            }
            contDescendente = 0; //reseteo contador
         }
         contDescendente++; //incremento
      break;

      default:
         estadoActual = ALTO; 
      break;
   }

    
}

