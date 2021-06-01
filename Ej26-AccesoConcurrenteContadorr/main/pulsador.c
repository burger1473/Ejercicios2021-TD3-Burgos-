/*==================[ Inclusiones ]============================================*/
#include "../include/pulsador.h"


/*==================[ Definiciones ]===================================*/

#define T_REBOTE_MS   40
#define T_REBOTE pdMS_TO_TICKS(T_REBOTE_MS)
//#define SALIDA_PRUEBA   GPIO_NUM_26
#define cantidad_pulsadores 2

/*==================[Variables]==============================*/
gpio_int_type_t pulsadorPines[cantidad_pulsadores] = { 12, 13 };

pulsadorInfo pulsador[cantidad_pulsadores];

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED; //Inicializa el spinlock desbloqueado

/*==================[Prototipos de funciones]======================*/

static void errorPulsador( uint8_t i );

void tarea_A( void* taskParmPtr );

void botonLiberado(uint8_t i);


/*==================[Implementaciones]=================================*/

void inicializarPulsador( void)
{
    for(uint8_t i=0; i<cantidad_pulsadores; i++){
        pulsador[i].tecla = pulsadorPines[i];
        pulsador[i].estado = ALTO;                     //Estado inicial

        gpio_pad_select_gpio(pulsador[i].tecla);
        gpio_set_direction(pulsador[i].tecla , GPIO_MODE_INPUT);
        gpio_set_pull_mode(pulsador[i].tecla, GPIO_PULLDOWN_ONLY); //Habilita resistencia de PULLDOWN interna
    }
    
 
    // Crear tareas en freeRTOS
    BaseType_t res = xTaskCreatePinnedToCore(
    	tarea_A,                     	// Funcion de la tarea a ejecutar
        "tarea_A",   	                // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
        NULL,                          	// Parametros de tarea
        tskIDLE_PRIORITY,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
        NULL,                          	// Puntero a la tarea creada en el sistema
        0                               // Ejecuto desde procesador A
    );

    // Gestion de errores
	if(res == pdFAIL)
	{
		printf( "Error al crear la tarea A.\r\n" );
		while(true);					// si no pudo crear la tarea queda en un bucle infinito
	}
}

static void errorPulsador( uint8_t i)
{
    pulsador[i].estado = ALTO;
}

// Funcion para actualizar estado del pulsador
void actualizarPulsador(uint8_t i)
{
    switch( pulsador[i].estado )
    {
        case BAJO:
            if( gpio_get_level( pulsador[i].tecla ) ){ //si el pulsador es un 1 logico, pasa a estado ascendente
                pulsador[i].estado = ASCENDENTE;                
            }
            break;

        case ASCENDENTE:
            if( gpio_get_level( pulsador[i].tecla ) ){  //Si el pulsador permace en 1 logico, pasa a estado alto
                pulsador[i].estado = ALTO;
            }
            else{  //Si esta en 0 logico, fue un rebote y volve a estado bajo
                pulsador[i].estado = BAJO;
            }
            break;

        case ALTO:
            if( !gpio_get_level( pulsador[i].tecla ) ){ //Espera a que el pulsador este en cero logico
                pulsador[i].estado = DESCENDENTE;  //Entra en modo desendente
            }
            break;

        case DESCENDENTE:
            if( !gpio_get_level( pulsador[i].tecla ) ){ //Si el pulsador sigue en cero logico
                pulsador[i].estado = BAJO; //pasa a estado bajo
                botonLiberado(i); //establece en la estructura del pulsador, el tiempo que estuvo en estado alto
            }
            else{ //Si el pulsador esta en 1 logico, fue un rebote y vuelve al estado Alto
                pulsador[i].estado = ALTO;
            }
            break;

        default:
            errorPulsador(i);
            break;
    }
}

//Funcion para incrementar o decrementar
//Envio parmetro del pulsador libreado, si es 0 es el pulsador de incremento
//                                      si es 1 es el pulsador de decremento
void botonLiberado(uint8_t i){

    portENTER_CRITICAL(&mux);
    if(i==0){
        contador=contador+100;
    }

    if(i==1){
        contador=contador-100;
    }

    if(contador>900){contador=900;}

    if(contador<100){contador=100;}
    portEXIT_CRITICAL(&mux);
}



void tarea_A( void* taskParmPtr )
{
    while( true ) //bucle infinito
    {
        for(uint8_t i=0; i<cantidad_pulsadores; i++){
            actualizarPulsador(i); //Actualiza el pulsador y obtiene el tiempo en alto
        }
        vTaskDelay( T_REBOTE ); //Timpo para evitar rebote
    }
}