/*==================[ Inclusiones ]============================================*/
#include "../include/led.h"
#include "../include/pulsador.h"

/*==================[ Definiciones ]===================================*/
#define SALIDA1     4
#define PROCESADORA 0
#define PROCESADORB 1
#define PERIODO_MS  1000
#define PERIODO     pdMS_TO_TICKS(PERIODO_MS)

/*==================[Prototipos de funciones]======================*/

void tareaLED( void* taskParmPtr ); //Prototipo de la función de la tarea

/*==================[Variables]==============================*/

TaskHandle_t xHandle_tareaLED=NULL; //Puntero hacia tareaLED

/*==================[Implementaciones]=================================*/
void crear_TareaLED(){
    if(xHandle_tareaLED==NULL){ //Si la tarea no se creo, la creo
        BaseType_t res = xTaskCreatePinnedToCore(
                    tareaLED,                     	// Funcion de la tarea a ejecutar
                    "tareaLED",   	                // Nombre de la tarea como String amigable para el usuario
                    configMINIMAL_STACK_SIZE*2, 		// Cantidad de stack de la tarea
                    NULL,                          	// Parametros de tarea
                    tskIDLE_PRIORITY+1,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
                    &xHandle_tareaLED,              // Puntero a la tarea creada en el sistema
                    PROCESADORA                     //Ejecuto en procesador A
        );

                // Gestion de errores
                if(res == pdFAIL)
                {
                    printf( "Error al crear la tarea.\r\n" );
                    while(true);					// si no pudo crear la tarea queda en un bucle infinito
                }
    }
}

// Implementacion de funcion de la tarea
void tareaLED( void* taskParmPtr )
{
    // ---------- Congiguraciones------------------------------
    gpio_pad_select_gpio(SALIDA1);
    gpio_set_direction(SALIDA1, GPIO_MODE_OUTPUT);

    TickType_t xPeriodicity =  PERIODO;			// Tarea periodica cada 1000 ms

    TickType_t xLastWakeTime = xTaskGetTickCount();

     while( true )
    {
        gpio_set_level(SALIDA1, 1);
        if ( dif >= PERIODO_MS)
        {
            dif = 0;
        }

        vTaskDelay (pdMS_TO_TICKS(dif));
                
        gpio_set_level(SALIDA1, 0);
        // Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico)
        vTaskDelayUntil( &xLastWakeTime , xPeriodicity );
    }
}