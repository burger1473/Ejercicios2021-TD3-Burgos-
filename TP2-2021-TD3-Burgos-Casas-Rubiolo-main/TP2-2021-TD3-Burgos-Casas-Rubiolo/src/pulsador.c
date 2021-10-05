#include "pulsador.h"

//========================== Funciones ====================================
void IRAM_ATTR pulsador_isr_handler_inc(void* arg);     //inc-> incremento
void IRAM_ATTR pulsador_isr_handler_dec(void* arg);     //dec-> decremento
void tareaIncremento( void* taskParmPtr );
void tareaDecremento( void* taskParmPtr );

//===================== Variables ====================================
int8_t resta_1 = -1;
int8_t suma_1 = 1;
extern QueueHandle_t cola[];

/*
*   Configuracion de los pines para interrupcion
*/
void config_pulsador_int()
{
    for ( uint32_t INDICE = 0; INDICE < N_LED; INDICE++)
    {
        //configuro el PIN_PULSADOR como un pin GPIO
        gpio_pad_select_gpio(Pulsador_Incremento[INDICE]);
        // seleciono el PIN_PULSADOR como pin de entrada
        gpio_set_direction(Pulsador_Incremento[INDICE], GPIO_MODE_INPUT);
        
        //Habilita resistencia de PULLDOWN interna
        gpio_set_pull_mode(Pulsador_Incremento[INDICE], GPIO_PULLDOWN_ONLY);

        /*
        * GPIO set interrupt trigger type establece el tipo de disparo de interrupción
        *
        * gpio_set_intr_type(gpio_num_t "gpio_num", gpio_int_type_t "intr_type");
        * 
        * @param gpio_num:  Para establecer el tipo de activacion de tal pin, Ej.: GPIO_NUM_16);
        * @param intr_type: Para establecer el tipo de interrupcion(ascendente, descendente, etc.)
        * 
        *   GPIO_INTR_DISABLE = 0,      Disable GPIO interrupt
        *   GPIO_INTR_POSEDGE = 1,      !< GPIO interrupt type : rising edge
        *   GPIO_INTR_NEGEDGE = 2,      !< GPIO interrupt type : falling edge 
        *   GPIO_INTR_ANYEDGE = 3,      !< GPIO interrupt type : both rising and falling edge
        *   GPIO_INTR_LOW_LEVEL = 4,    !< GPIO interrupt type : input low level trigger
        *   GPIO_INTR_HIGH_LEVEL = 5,   !< GPIO interrupt type : input high level trigger
        * 
        */
        gpio_set_intr_type(Pulsador_Incremento[INDICE], GPIO_INTR_POSEDGE);// habilito interrupción por flanco Ascendente (1->INDICE)
        
        /*
        *   Instala el servicio de controlador GPIO ISR del controlador, que permite controladores 
        *   de interrupciones GPIO por pin.
        *   Esta función es incompatible con gpio_isr_register(); si se usa esa función, se registra
        *   un único ISR global para todas las interrupciones GPIO. Si se utiliza esta función, el 
        *   servicio ISR proporciona un GPIO ISR global y los manejadores de pines individuales se 
        *   registran a través de la función gpio_isr_handler_add()
        */
        gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

        /*
        *   Llame a esta función después de usar gpio_install_isr_service () para instalar el servicio 
        *   de controlador GPIO ISR del controlador. Ya no es necesario declarar los controladores pin 
        *   ISR con IRAM_ATTR, a menos que pase el indicador ESP_INTR_FLAG_IRAM al asignar el ISR en 
        *   gpio_install_isr_service (). Este controlador de ISR se llamará desde un ISR. Así que hay 
        *   un límite de tamaño de pila (configurable como "tamaño de pila ISR" en menuconfig). Este límite 
        *   es más pequeño en comparación con un controlador de interrupciones GPIO global debido al nivel 
        *   adicional de indirección.
        *   
        *   gpio_isr_handler_add(gpio_num_t gpio_num, gpio_isr_t isr_handler, void *args);
        * 
        *   @param gpio_num:    Pin a utilizar.
        *   @param isr_handler: Función de controlador ISR para el número GPIO correspondiente.
        *   @param args:        parámetro para el controlador ISR.
        */
        gpio_isr_handler_add(Pulsador_Incremento[INDICE], pulsador_isr_handler_inc, (void*)INDICE);
    }


    for ( uint32_t INDICE = 0; INDICE < N_LED; INDICE++)
    {
        //configuro el PIN_PULSADOR como un pin GPIO
        gpio_pad_select_gpio(Pulsador_Decremento[INDICE]);
        // seleciono el PIN_PULSADOR como pin de entrada
        gpio_set_direction(Pulsador_Decremento[INDICE], GPIO_MODE_INPUT);
        // instala el servicio ISR con la configuración por defecto.
        gpio_set_pull_mode(Pulsador_Decremento[INDICE], GPIO_PULLDOWN_ONLY); //Habilita resistencia de PULLDOWN interna

        /*
        * GPIO set interrupt trigger type establece el tipo de disparo de interrupción
        *
        * gpio_set_intr_type(gpio_num_t "gpio_num", gpio_int_type_t "intr_type");
        * 
        * @param gpio_num:  Para establecer el tipo de activacion de tal pin, Ej.: GPIO_NUM_16);
        * @param intr_type: Para establecer el tipo de interrupcion(ascendente, descendente, etc.)
        * 
        *   GPIO_INTR_DISABLE = 0,      Disable GPIO interrupt
        *   GPIO_INTR_POSEDGE = 1,      !< GPIO interrupt type : rising edge
        *   GPIO_INTR_NEGEDGE = 2,      !< GPIO interrupt type : falling edge 
        *   GPIO_INTR_ANYEDGE = 3,      !< GPIO interrupt type : both rising and falling edge
        *   GPIO_INTR_LOW_LEVEL = 4,    !< GPIO interrupt type : input low level trigger
        *   GPIO_INTR_HIGH_LEVEL = 5,   !< GPIO interrupt type : input high level trigger
        * 
        */
        gpio_set_intr_type(Pulsador_Decremento[INDICE], GPIO_INTR_POSEDGE);
        
        /*
        *   Instala el servicio de controlador GPIO ISR del controlador, que permite controladores 
        *   de interrupciones GPIO por pin.
        *   Esta función es incompatible con gpio_isr_register(); si se usa esa función, se registra
        *   un único ISR global para todas las interrupciones GPIO. Si se utiliza esta función, el 
        *   servicio ISR proporciona un GPIO ISR global y los manejadores de pines individuales se 
        *   registran a través de la función gpio_isr_handler_add()
        */
        gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
        
        /*
        *   Llame a esta función después de usar gpio_install_isr_service () para instalar el servicio 
        *   de controlador GPIO ISR del controlador. Ya no es necesario declarar los controladores pin 
        *   ISR con IRAM_ATTR, a menos que pase el indicador ESP_INTR_FLAG_IRAM al asignar el ISR en 
        *   gpio_install_isr_service (). Este controlador de ISR se llamará desde un ISR. Así que hay 
        *   un límite de tamaño de pila (configurable como "tamaño de pila ISR" en menuconfig). Este límite 
        *   es más pequeño en comparación con un controlador de interrupciones GPIO global debido al nivel 
        *   adicional de indirección.
        *   
        *   gpio_isr_handler_add(gpio_num_t gpio_num, gpio_isr_t isr_handler, void *args);
        * 
        *   @param gpio_num:    Pin a utilizar.
        *   @param isr_handler: Función de controlador ISR para el número GPIO correspondiente.
        *   @param args:        parámetro para el controlador ISR.
        */
        gpio_isr_handler_add(Pulsador_Decremento[INDICE], pulsador_isr_handler_dec, (void*)INDICE);
    }
}

/*
*   Rutina de interrupción, llamada cuando se presiona el pulsador para incrementar
*   recibe el indice del pulsador presionado y crea una tarea para determinar si el
*   pulso es o no ruido. Luego decrementa el duty y se elimina la tarea.
*/
void IRAM_ATTR pulsador_isr_handler_inc(void* arg) {

    //  El parametro que recibe cada interrupcion es el del pulsador
    //  presionado. Guarda dicho parametro en la variable INDICE y crea
    //  una tarea que hara de debounce y determinara que duty modificar
    //  dependiendo del parametro recibido
    uint32_t INDICE = (uint32_t) arg;

    //  Una vez se interrumpe desactivo el pin que genero dicha interrupcion 
    //  para evitar que se generen nuevas interrupciones y tareas hasta que
    //  se finalize la siguiente tarea.
    gpio_isr_handler_remove(Pulsador_Incremento[INDICE]);

    //  Tarea para incrementar el duty si no es ruido. Envia como parametro el
    //  indice de pulsador presionado.
    BaseType_t res = xTaskCreatePinnedToCore(
    	tareaIncremento,                // Funcion de la tarea a ejecutar
        "tareaIncremento",   	        // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*4, 	// Cantidad de stack de la tarea
        (void*) INDICE,                 // Parametros de tarea(debe ser convertido a puntero previamente)
        tskIDLE_PRIORITY+2,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
        NULL,                          	// Puntero a la tarea creada en el sistema
        1
    );

    // Gestion de errores
	if(res == pdFAIL)
	{
		printf( "Error al crear la tarea.\r\n" );
		while(true);					// si no pudo crear la tarea queda en un bucle infinito
	}
}

/*
*   Rutina de interrupción, llamada cuando se presiona el pulsador para decrementar
*   recibe el indice del pulsador presionado y crea una tarea para determinar si el
*   pulso es o no ruido. Luego decrementa el duty y se elimina la tarea.
*/
void IRAM_ATTR pulsador_isr_handler_dec(void* arg) {

    //  El parametro que recibe cada interrupcion es el del pulsador
    //  presionado. Guarda dicho parametro en la variable INDICE y cre
    //  una tarea que hara de debounce y determinara que duty modificar
    //  dependiendo del parametro recibido
    uint32_t INDICE = (uint32_t) arg;

    //  Una vez se interrumpe desactivo el pin que genero dicha interrupcion 
    //  para evitar que se generen nuevas interrupciones y tareas hasta que
    //  se finalize la siguiente tarea.
    gpio_isr_handler_remove(Pulsador_Decremento[INDICE]);

    //  Tarea para decrementar el duty si no es ruido. Envia como parametro el
    //  indice de pulsador presionado.
    BaseType_t res = xTaskCreatePinnedToCore(
    	tareaDecremento,                // Funcion de la tarea a ejecutar
        "tareaDecremento",   	        // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*4, 	// Cantidad de stack de la tarea
        (void*) INDICE,                 // Parametros de tarea(debe ser convertido a puntero previamente)
        tskIDLE_PRIORITY+3,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
        NULL,                          	// Puntero a la tarea creada en el sistema
        1
    );
   
    // Gestion de errores
	if(res == pdFAIL)
	{
		printf( "Error al crear la tarea.\r\n" );
		while(true);					// si no pudo crear la tarea queda en un bucle infinito
	}
}

//  Tarea para incrementar el duty si no es ruido. Recibe como parametro el
//  indice de pulsador presionado. Una vez finalizada la tarea esta se elimina
void tareaIncremento( void* taskParmPtr )
{
    //  El parametro que recibe es el del pulsador presionado y guarda 
    //  dicho parametro en la variable INDICE
    uint32_t INDICE = ( uint32_t ) taskParmPtr;

    //  Delay para evitar rebote. Si finalizado este, el pulsador se encuentra
    //  en estado bajo significa que la interrupcion recibida fue ruido y no 
    //  incrementa el duty, caso contrario, lo incrementa.
    vTaskDelay(pdMS_TO_TICKS(100));

	if(gpio_get_level(Pulsador_Incremento[INDICE]) == 1){
        // Codigo
        xQueueSend( cola[INDICE] , &suma_1,  portMAX_DELAY );
	}

    //  Una vez finalizada la tarea activa la interrupcion para el pin
    //  recibido y elimina la tarea
    gpio_isr_handler_add(Pulsador_Incremento[INDICE], pulsador_isr_handler_inc, (void*)INDICE);

	vTaskDelete( NULL );
}

//  Tarea para decremantar el duty si no es ruido. Recibe como parametro el
//  indice de pulsador presionado. Una vez finalizada la tarea esta se elimina
void tareaDecremento( void* taskParmPtr )
{
    //  El parametro que recibe es el del pulsador presionado y guarda 
    //  dicho parametro en la variable INDICE
    uint32_t INDICE = ( uint32_t ) taskParmPtr;

    //  Delay para evitar rebote. Si finalizado este, el pulsador se encuentra
    //  en estado bajo significa que la interrupcion recibida fue ruido y no 
    //  decrementa el duty, caso contrario, lo decrementa.
    vTaskDelay(pdMS_TO_TICKS(100));

	if(gpio_get_level(Pulsador_Decremento[INDICE]) == 1){
        // codigo
        xQueueSend( cola[INDICE] , &resta_1,  portMAX_DELAY );
	}

    //  Una vez finalizada la tarea activa la interrupcion para el pin
    //  recibido y elimina la tarea
    gpio_isr_handler_add(Pulsador_Decremento[INDICE], pulsador_isr_handler_dec, (void*)INDICE);

	vTaskDelete( NULL );
}