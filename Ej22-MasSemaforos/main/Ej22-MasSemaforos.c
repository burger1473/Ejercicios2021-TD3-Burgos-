/*=============================================================================
 * Author: Fabian Burgos
 * Date: 10/05/2021 
 * Board: ESP32-CAM
 * Ide: IDF 
 * Pin 4 led VERDE
 * Pin 16 led Rojo
 * Pin 14 Pulsador
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
#include "freertos/semphr.h" //Libreria para uso de semaforo

/*==================[ Definiciones ]===================================*/
#define SALIDA1     4
#define SALIDA2     33
#define LED_ON_MS    500
#define LED_ON       pdMS_TO_TICKS(LED_ON_MS)
#define PERIODO_MS  1000
#define PERIODO     pdMS_TO_TICKS(PERIODO_MS)
/*==================[Variables]==============================*/
SemaphoreHandle_t semaforo = NULL; //Puntero al semaforo

/*==================[Prototipos de funciones]======================*/
void tarea_B( void* taskParmPtr );

void app_main()
{
    inicializarLed(SALIDA1);//Configuro como salida
    inicializarLed(SALIDA2);//Configuro como salida
     //Crear el semáforo (arranca “tomado”)
    semaforo = xSemaphoreCreateBinary();
    if(semaforo == NULL)
    {
        printf("No se pudo crear el semaforo\n");
        while(true);
    }
    
    inicializarPulsador();//Crea una tarea con priorirdad tskIDLE_PRIORITY+1 para actualizar el estado del pulsador
    
    // Crear tareas en freeRTOS
    BaseType_t res = xTaskCreatePinnedToCore(
    	tarea_B,                     	// Funcion de la tarea a ejecutar
        "tarea_B",   	                // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
        NULL,                          	// Parametros de tarea
        tskIDLE_PRIORITY+1,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
        NULL,                          	// Puntero a la tarea creada en el sistema
        0                               // Ejecuto desde procesador B
    );

    // Gestion de errores
	if(res == pdFAIL)
	{
		printf( "Error al crear la tarea.\r\n" );
		while(true);					// si no pudo crear la tarea queda en un bucle infinito
	}
    printf( "Fin Main\r\n" );
}


void tarea_B( void* taskParmPtr )
{
    while( true )
    {
        TickType_t xPeriodicity =  PERIODO;	//Periodo de 1 segundo

        TickType_t xLastWakeTime = xTaskGetTickCount(); //Obtiene los tick actuales

        if(xSemaphoreTake(semaforo, pdMS_TO_TICKS(1)) == pdTRUE){
            ApagarLed(SALIDA1);
            PrenderLed(SALIDA1);
            vTaskDelay( LED_ON );
            ApagarLed(SALIDA1);
        }else{
            ApagarLed(SALIDA1);
            PrenderLed(SALIDA2);
            vTaskDelay( LED_ON );
            ApagarLed(SALIDA2);
        }

        vTaskDelayUntil( &xLastWakeTime , xPeriodicity ); //queda esperando el tiempo necesario para hacer un periodo
    }
}
