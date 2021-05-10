/*=============================================================================
 * Author: Fabian Burgos
 * Date: 10/05/2021 
 * Board: ESP32-CAM 
 * Pin 4 led
 * Pin 14 Pulsador
 *===========================================================================*/
#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"
#include "driver/gpio.h"
#include "../include/pulsador.h"



void app_main()
{
    // Crear tarea en freeRTOS
    // Devuelve pdPASS si la tarea fue creada y agregada a la lista ready
    // En caso contrario devuelve pdFAIL.
    inicializarPulsador();//Crea una tarea con priorirdad tskIDLE_PRIORITY+1 para actualizar el estado del pulsador y obtener el tiempo en alto
    Inicializar_Led();
    // Dentro de la tareaPulsador al soltar el pulsador, crea la tareaLed la cual prnde y apaga el led y ademas
        // se autodestruye al pasar el tiempo igual al de la pulsacion del pulsador.
    // Decidi realizarlo adentro de la tareaPulsador, para no mantener la tarea main u otra tarea gastando recursos sin ser necesario. 
    
}

