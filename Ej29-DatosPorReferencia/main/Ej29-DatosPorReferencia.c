/*=============================================================================
 * Author: Fabian Burgos
 * Date: 07/06/2021 
 * Board: ESP32-CAM
 * Ide: IDF 
 * Pin 16 led -> se modifica en este archivo
 * Pin 12 Pulsador -> se modifica en pulsador.c
 * El codigo esta realizado para ingresar la cantidad de leds y pulsadores que se deseen
 * de esta forma segun el pulsador que se presione, se prendera su led correspondiente
 * con periodo igual a 1 seg y tiempo en alto igual al tiempo de presion del pulsador.
 *===========================================================================*/

/*==================[ Inclusiones ]============================================*/
#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"
#include "driver/gpio.h"
#include "../include/pulsador.h"
#include "../include/led.h"


/*==================[ Definiciones ]===================================*/
#define PERIODO_MS  1000
#define PERIODO     pdMS_TO_TICKS(PERIODO_MS)
#define cantidad_led 1

/*==================[Variables]==============================*/
extern pulsadorInfo pulsador[];
gpio_int_type_t Salida[cantidad_led] = { 16 };


/*==================[Prototipos de funciones]======================*/
void tarea_B( void* taskParmPtr );

/*==================[Implementaciones]=================================*/
void app_main()
{
   for(uint8_t i=0; i<cantidad_led; i++){
    inicializarLed(Salida[i]);//Configuro como salida
   }
   inicializarPulsador();//Crea una tarea con priorirdad tskIDLE_PRIORITY+1 para actualizar el estado del pulsador
    
    // Crear tareas en freeRTOS
    BaseType_t res = xTaskCreatePinnedToCore(
    	tarea_B,                     	// Funcion de la tarea a ejecutar
        "tarea_B",   	                // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*4, 	// Cantidad de stack de la tarea
        NULL,                          	// Parametros de tarea
        tskIDLE_PRIORITY,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
        NULL,                          	// Puntero a la tarea creada en el sistema
        0                               // Ejecuto desde procesador B
    );

    // Gestion de errores
	if(res == pdFAIL)
	{
		printf( "Error al crear la tarea B.\r\n" );
		while(true);					// si no pudo crear la tarea queda en un bucle infinito
	}

       printf( "Fin Main\r\n" );
   
}


void tarea_B( void* taskParmPtr )
{
    // ---------- Configuraciones ------------------------------
    TickType_t xPeriodicity =  PERIODO;
	TickType_t xLastWakeTime = xTaskGetTickCount();

    TickType_t dif = 0;
    estructura_t* parametros_recibidos; //Variable para almacenar el puntero de la estructura

    uint8_t led=99;//Establesco 99 como valor basura

    while( true )
    {
        if(xQueueReceive( pulsador[0].cola , &parametros_recibidos,  0 )) //recibo dato desde la cola sin tiempo de espera para no bloquearse
        {
            //acá se recibió
            dif = parametros_recibidos->tiempo;
            led = parametros_recibidos->indice_Led;
           
            //Establesco valor maximo
            if( dif > xPeriodicity )
            {
                dif = xPeriodicity;
            }
        }
        else
        {
            //acá no se recibió
        }
         printf( "Indice: %u \n", led); //Para verificar en monitor serial

        if(led!=99){ //si el valor del led es diferente al dato basura, es decir que recibio el dato del led desde la cola al menos una vez
            PrenderLed(Salida[led]);
            vTaskDelay( dif );
            ApagarLed(Salida[led]);

            vTaskDelayUntil( &xLastWakeTime , xPeriodicity );     
        }
        
    }
}