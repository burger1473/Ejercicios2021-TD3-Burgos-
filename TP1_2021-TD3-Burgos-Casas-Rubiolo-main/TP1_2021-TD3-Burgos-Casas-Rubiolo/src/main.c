/*=============================================================================
 * Author: Fabian Burgos, Bruno Rubiolo, Alejo Casas.
 * Date: 28/04/2021 
 * Board: ESP32
 * TP: TP1_2021-TD3-Burgos-Casas-Rubiolo
 * 
 *      El programa esta realizado para variar la cantidad de participantes del
 *   grupo de una manera sencilla.
 *      Se debe modificar el Define Cantidad_Participantes dentro de Global.h
 *      Se debe modificar el arreglo Salida y Entrada con los pines correspondientes
 * 
 *===========================================================================*/



#define T 5/portTICK_PERIOD_MS

#include "../include/Global.h"
#include "../include/Pulsador.h"
#include "../include/Led.h"

uint8_t Entrada[Cantidad_Participantes] = {GPIO_NUM_15, GPIO_NUM_2, GPIO_NUM_4 }; // Especifico pines de entrada

uint8_t Salida[Cantidad_Participantes] = {GPIO_NUM_21, GPIO_NUM_22, GPIO_NUM_23 };// Especifico pines de salida

void app_main()
{
    iniciarLed();           // LLamo a la funcion iniciarLed
    iniciarPulsadores();    // LLamo a la funcion iniciarPulsadores
    iniciarContadores ();   // LLamo a la funcion iniciarContadores
 
    InicializarMEF();       // LLamo a la funcion InicializarMEF

    // BUCLE infinito

    while (1)
    {   
        actualizarPulsador();// Actualizo la MEF de cada participante
        vTaskDelay(T) ;      // Delay para retardo del contador 
    }
    
}