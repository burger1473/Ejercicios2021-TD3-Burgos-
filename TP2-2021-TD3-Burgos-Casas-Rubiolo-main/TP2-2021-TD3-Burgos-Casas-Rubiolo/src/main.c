/*==================[ Inclusiones ]============================================*/
#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"
#include "driver/gpio.h"
#include "freertos/queue.h" //incluyo la libreria para usar cola

/*==================[ Ficheros ]============================================*/
#include "global.h"
#include "Uart.h"
#include "pulsador.h"

//===================== Variables ====================================
uint8_t contador[N_LED];
gpio_int_type_t led [N_LED] = {GPIO_NUM_25, GPIO_NUM_26};
gpio_int_type_t Pulsador_Incremento[N_LED] = { GPIO_NUM_18, GPIO_NUM_19 };
gpio_int_type_t Pulsador_Decremento[N_LED] = { GPIO_NUM_22, GPIO_NUM_23 };
extern QueueHandle_t cola[];

/*==================[ Funciones ]============================================*/
void PWM( void* taskParmPtr ); //Prototipo de la función de la tarea

void app_main()
{   
    config_Uart();
    config_pulsador_int();

    for(int h=0; h<N_LED; h++){
        contador[h]=1;          //inicializo los contadores en 1ms
    }

    for(int i = 0; i< N_LED; i++){
        
        BaseType_t res = xTaskCreatePinnedToCore(
            PWM,                     	// Funcion de la tarea a ejecutar
            "Pwm",   	                // Nombre de la tarea como String amigable para el usuario
            configMINIMAL_STACK_SIZE*2, 		// Cantidad de stack de la tarea
            i,                          	// Parametros de tarea
            tskIDLE_PRIORITY+1+i,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
            NULL,                          		// Puntero a la tarea creada en el sistema
            PROCESADORA
        );

        // Gestion de errores
        if(res == pdFAIL)
        {
            //printf( "Error al crear la tarea.\r\n" );
            while(true);					// si no pudo crear la tarea queda en un bucle infinito
        }
    }

}

// Implementacion de funcion de la tarea
void PWM( void* taskParmPtr )
{
    uint8_t indice = ( uint8_t ) taskParmPtr;

    gpio_pad_select_gpio(led[indice]);
    gpio_set_direction(led[indice], GPIO_MODE_OUTPUT);

    TickType_t xPeriodicity =  PERIODO;			// Tarea periodica cada 10 ms
    TickType_t xLastWakeTime = xTaskGetTickCount();

    int8_t ardilla;

    // ---------- Bucle infinito --------------------------
    while( true )
    {   
        if(xQueueReceive( cola[indice] , &ardilla,  0 )) //recibo dato desde la cola sin tiempo de espera
        {   
            if(contador[indice]==0 && ardilla==-1){
                contador[indice]=0;
            } else if (contador[indice]==10 && ardilla==+1){
                contador[indice]=10;
            } else {
                contador[indice]=contador[indice]+ardilla;
            }
            
        }

        gpio_set_level(led[indice], 1);
        vTaskDelay ( pdMS_TO_TICKS( contador[indice]) );
        gpio_set_level(led[indice], 0);

        vTaskDelayUntil( &xLastWakeTime , xPeriodicity );
    }
}