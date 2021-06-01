/*=============================================================================
 * Author: Fabian Burgos
 * Date: 10/05/2021 
 * Board: ESP32-CAM
 * Ide: IDF 
 * Pin 4 led A
 * Pin 16 led B
 * Pin 14 Pulsador Incremento
 * Pin 15 pulsador Decremento
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
#define SALIDA1     16
#define SALIDA2     4
#define LED_ON_MS    500
#define LED_ON       pdMS_TO_TICKS(LED_ON_MS)
#define PERIODO_MS  2000
#define PERIODO     pdMS_TO_TICKS(PERIODO_MS)

/*==================[Variables]==============================*/
int contador=900;
extern portMUX_TYPE mux; 

/*==================[Prototipos de funciones]======================*/
void tarea_B( void* taskParmPtr );
void tarea_C( void* taskParmPtr );

void app_main()
{
    inicializarLed(SALIDA1);//Configuro como salida
    inicializarLed(SALIDA2);//Configuro como salida

   inicializarPulsador();//Crea una tarea con priorirdad tskIDLE_PRIORITY+1 para actualizar el estado del pulsador
    
    // Crear tareas en freeRTOS
    BaseType_t res = xTaskCreatePinnedToCore(
    	tarea_B,                     	// Funcion de la tarea a ejecutar
        "tarea_B",   	                // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
        NULL,                          	// Parametros de tarea
        tskIDLE_PRIORITY,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
        NULL,                          	// Puntero a la tarea creada en el sistema
        1                               // Ejecuto desde procesador B
    );

    // Gestion de errores
	if(res == pdFAIL)
	{
		printf( "Error al crear la tarea B.\r\n" );
		while(true);					// si no pudo crear la tarea queda en un bucle infinito
	}

    // Crear tareas en freeRTOS
    res = xTaskCreatePinnedToCore(
    	tarea_C,                     	// Funcion de la tarea a ejecutar
        "tarea_C",   	                // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
        NULL,                          	// Parametros de tarea
        tskIDLE_PRIORITY+1,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
        NULL,                          	// Puntero a la tarea creada en el sistema
        1                               // Ejecuto desde procesador B
    );

    // Gestion de errores
	if(res == pdFAIL)
	{
		printf( "Error al crear la tarea C.\r\n" );
		while(true);					// si no pudo crear la tarea queda en un bucle infinito
	}
    printf( "Fin Main\r\n" );
   
}


void tarea_B( void* taskParmPtr )
{
    while( true )
    {
        TickType_t xPeriodicity =  pdMS_TO_TICKS(contador);	//Periodo de 1 segundo

        TickType_t xLastWakeTime = xTaskGetTickCount(); //Obtiene los tick actuales
        PrenderLed(SALIDA1);
        vTaskDelay((contador/2)/ portTICK_PERIOD_MS);//50% siclo util
        ApagarLed(SALIDA1);
        vTaskDelayUntil( &xLastWakeTime , xPeriodicity ); //queda esperando el tiempo necesario para hacer un periodo
    }
}

void tarea_C( void* taskParmPtr )
{
    while( true )
    {
        TickType_t xPeriodicity =  PERIODO;	//Periodo de 2 segundo

        TickType_t xLastWakeTime = xTaskGetTickCount(); //Obtiene los tick actuales
        PrenderLed(SALIDA2);
        vTaskDelay((contador*2)/ portTICK_PERIOD_MS);
        ApagarLed(SALIDA2);
        vTaskDelayUntil( &xLastWakeTime , xPeriodicity ); //queda esperando el tiempo necesario para hacer un periodo
        portENTER_CRITICAL(&mux);
        contador=contador-100;
        if(contador<100){contador=100;}
        portEXIT_CRITICAL(&mux);
    
    }
}
