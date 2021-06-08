/*==================[ Inclusiones ]============================================*/
#include "../include/pulsador.h"


/*==================[ Definiciones ]===================================*/

#define T_REBOTE_MS   40
#define T_REBOTE pdMS_TO_TICKS(T_REBOTE_MS)
//#define SALIDA_PRUEBA   GPIO_NUM_26
#define cantidad_pulsadores 1



static estructura_t parametros; //Variable del tipo estructura

/*==================[Variables]==============================*/
gpio_int_type_t pulsadorPines[cantidad_pulsadores] = { 12 };

pulsadorInfo pulsador[cantidad_pulsadores];


/*==================[Prototipos de funciones]======================*/

static void errorPulsador( uint8_t i );

void tarea_A( void* taskParmPtr );

static void botonPresionado(uint8_t  i);

static void botonLiberado(uint8_t i);


/*==================[Implementaciones]=================================*/


TickType_t obtenerDiferencia(uint8_t  i)
{
    TickType_t tiempo;
    tiempo = pulsador[i].diferenciaTiempo;
    return tiempo;
}

void borrarDiferencia( uint8_t  i )
{
    pulsador[i].diferenciaTiempo = TIEMPO_NO_VALIDO;
}

void inicializarPulsador( void)
{
    for(uint8_t i=0; i<cantidad_pulsadores; i++){
        pulsador[i].tecla = pulsadorPines[i];
        pulsador[i].estado = ALTO;                     //Estado inicial
        pulsador[i].tiempoBajo        = TIEMPO_NO_VALIDO;
        pulsador[i].tiempoAlto        = TIEMPO_NO_VALIDO;
        pulsador[i].diferenciaTiempo  = TIEMPO_NO_VALIDO;
        pulsador[i].cola          = xQueueCreate( 1, sizeof(estructura_t*) ); // Creación de la cola

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
        tskIDLE_PRIORITY+1,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
        NULL,                          	// Puntero a la tarea creada en el sistema
        1                               // Ejecuto desde procesador A
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
                botonPresionado(i);
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

//Funcion para contar tiempo presionado
void botonLiberado(uint8_t i){
    TickType_t conteoTicksActuales = xTaskGetTickCount();   //Medimos el tiempo en ticks desde que inició el scheduler
    pulsador[i].tiempoAlto    = conteoTicksActuales;
    pulsador[i].diferenciaTiempo  = pulsador[i].tiempoAlto - pulsador[i].tiempoBajo; //Da el tiempo que el pulsador estuvo en estado alto
    parametros.indice_Led=i;
    parametros.tiempo=pulsador[i].diferenciaTiempo;
    estructura_t* puntero_parametros = &parametros; //Puntero a la estructura parametros
    xQueueSend( pulsador[i].cola , &puntero_parametros,  portMAX_DELAY ); // Envío dato a la cola con tiempo de retardo infinito, es decir, cuando la cola esta llena se bloquea hasta que se libere un espacio
}


//Funcion para establecer los ticks de inicio de pulsacion
static void botonPresionado( uint8_t  i)
{
    TickType_t conteoTicksActuales = xTaskGetTickCount();   //Medimos el tiempo en ticks desde que inició el scheduler
    pulsador[i].tiempoBajo = conteoTicksActuales;             //guardamos ese tiempo como referencia
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