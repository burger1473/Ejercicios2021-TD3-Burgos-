/*==================[ Inclusiones ]============================================*/
#include "../include/led.h"
#include "../include/pulsador.h"

/*==================[ Definiciones ]===================================*/
#define SALIDA1     4
#define PROCESADORA 0
#define PROCESADORB 1

/*==================[Prototipos de funciones]======================*/

void tareaLED( void* taskParmPtr ); //Prototipo de la función de la tarea

/*==================[Variables]==============================*/

TaskHandle_t xHandle_tareaLED=NULL; //Puntero hacia tareaLED

/*==================[Implementaciones]=================================*/
void crear_TareaLED(){
    
        BaseType_t res = xTaskCreatePinnedToCore(
                    tareaLED,                     	// Funcion de la tarea a ejecutar
                    "tareaLED",   	                // Nombre de la tarea como String amigable para el usuario
                    configMINIMAL_STACK_SIZE*3, 		// Cantidad de stack de la tarea
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

void Inicializar_Led(){
// ---------- Congiguraciones------------------------------
    gpio_pad_select_gpio(SALIDA1);
    gpio_set_direction(SALIDA1, GPIO_MODE_OUTPUT);
    gpio_set_level(SALIDA1,0);
}

// Implementacion de funcion de la tarea
void tareaLED( void* taskParmPtr )
{
        
    while(true){
        printf( "prendido" );
        gpio_set_level( SALIDA1, 1 );
        vTaskDelay(dif);  //Delay con bloqueo del procesador, hasta que no se termine este retardo, el procesador no sigue con ninguna tarea
        gpio_set_level( SALIDA1, 0 );
        printf( "apagado" );
        borrarDiferencia(); //Seteo el tiempo en alto como valor cero
        //xHandle_tareaLED=NULL; //Borro el puntero ya que la tarea se elimina automaticamente al no tener un bucle infinito
        //vTaskDelete(xHandle_tareaLED);
        vTaskDelete(NULL);
    }
    
}