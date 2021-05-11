/*==================[ Inclusiones ]============================================*/
#include "../include/pulsador.h"


/*==================[ Definiciones ]===================================*/

#define T_REBOTE_MS   40
#define T_REBOTE pdMS_TO_TICKS(T_REBOTE_MS)
//#define SALIDA_PRUEBA   GPIO_NUM_26

/*==================[Prototipos de funciones]======================*/

static void errorPulsador( void );

void tarea_A( void* taskParmPtr );



/*==================[Variables]==============================*/
gpio_int_type_t pulsadorPines[1] = { 14 };

pulsadorInfo pulsadorA;

/*==================[Implementaciones]=================================*/

void inicializarPulsador( void )
{
    pulsadorA.tecla             = pulsadorPines[0];
    pulsadorA.estado            = ALTO;                     //Estado inicial

    gpio_pad_select_gpio(pulsadorA.tecla);
    gpio_set_direction(pulsadorA.tecla , GPIO_MODE_INPUT);
    gpio_set_pull_mode(pulsadorA.tecla, GPIO_PULLDOWN_ONLY); //Habilita resistencia de PULLDOWN interna

    //gpio_pad_select_gpio(SALIDA_PRUEBA);                //replica lo que hace el pulsador
    //gpio_set_direction(SALIDA_PRUEBA, GPIO_MODE_OUTPUT);
 
    // Crear tareas en freeRTOS
    BaseType_t res = xTaskCreatePinnedToCore(
    	tarea_A,                     	// Funcion de la tarea a ejecutar
        "tarea_A",   	                // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
        NULL,                          	// Parametros de tarea
        tskIDLE_PRIORITY+1,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
        NULL,                          	// Puntero a la tarea creada en el sistema
        1                               // Ejecuto desde procesador B
    );

    // Gestion de errores
	if(res == pdFAIL)
	{
		printf( "Error al crear la tarea.\r\n" );
		while(true);					// si no pudo crear la tarea queda en un bucle infinito
	}
}

static void errorPulsador( void )
{
    pulsadorA.estado = ALTO;
}

// pulsador_ Update State Function
void actualizarPulsador()
{
    switch( pulsadorA.estado )
    {
        case BAJO:
            if( gpio_get_level( pulsadorA.tecla ) ){ //si el pulsador es un 1 logico, pasa a estado ascendente
                pulsadorA.estado = ASCENDENTE;
            }
            break;

        case ASCENDENTE:
            if( gpio_get_level( pulsadorA.tecla ) ){  //Si el pulsador permace en 1 logico, pasa a estado alto
                pulsadorA.estado = ALTO;
            }
            else{  //Si esta en 0 logico, fue un rebote y volve a estado bajo
                pulsadorA.estado = BAJO;
            }
            break;

        case ALTO:
            if( !gpio_get_level( pulsadorA.tecla ) ){ //Espera a que el pulsador este en cero logico
                pulsadorA.estado = DESCENDENTE;  //Entra en modo desendente
            }
            break;

        case DESCENDENTE:
            if( !gpio_get_level( pulsadorA.tecla ) ){ //Si el pulsador sigue en cero logico
                pulsadorA.estado = BAJO; //pasa a estado bajo
                //botonLiberado(); //establece en la estructura del pulsador, el tiempo que estuvo en estado alto
                xSemaphoreGive(semaforo);
            }
            else{ //Si el pulsador esta en 1 logico, fue un rebote y vuelve al estado Alto
                pulsadorA.estado = ALTO;
            }
            break;

        default:
            errorPulsador();
            break;
    }
}





void tarea_A( void* taskParmPtr )
{
    while( true ) //bucle infinito
    {
        actualizarPulsador(); //Actualiza el pulsador y obtiene el tiempo en alto
        vTaskDelay( T_REBOTE ); //Timpo para evitar rebote
    }
}