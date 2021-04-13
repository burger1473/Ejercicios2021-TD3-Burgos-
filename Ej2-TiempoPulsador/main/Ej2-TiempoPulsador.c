/*=============================================================================
 * Author: Fabian Burgos
 * Date: 12/04/2021 
 * Board: ESP32-CAM
 * Respuesrta:
 *  ¿Qué sucede si mientras estoy pulsando una tecla, comienzo a pulsar otra?
 *      Se cuenta el tiempo de ambos pulsadores con un error minimo debido al tiempo de cada intruccion
 *  ¿Qué sucede si mientras el LED está encendido mostrando el tiempo de una tecla, presiona otra tecla (o la misma)?
 *      Apaga el led y le suma al tiempo que le quedo, el tiempo de pulsacion
 *===========================================================================*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "soc/soc.h" //disable brownout detector
#include "soc/rtc_cntl_reg.h" //disable brownout detector (deteccion de apagon)
#include "soc/rtc_wdt.h"


#define LED1 4
#define Cantidad_Salidas		2
#define Cantidad_Entradas		2

int8_t Salida[Cantidad_Salidas] = {4, 2}; //especifico pines de salida
int8_t Entrada[Cantidad_Entradas] = {16, 0}; //especifico pines de entrada
int contador[Cantidad_Salidas] = {0, 0};

void app_main() {
	//Configuración
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
    rtc_wdt_protect_off();
    rtc_wdt_disable();
	
	//Configuro salidas y las pongo en estado bajo
    for(int8_t i = 0; i < Cantidad_Salidas; i++){
		gpio_pad_select_gpio(Salida[i]);
		gpio_set_direction(Salida[i], GPIO_MODE_OUTPUT);
        gpio_set_level(Salida[i], 0);
	}
	
	//Configuro entradas y las pongo pulldown
    for(int8_t j = 0; j < Cantidad_Entradas; j++){
		gpio_pad_select_gpio(Entrada[j]);
		gpio_set_direction(Entrada[j], GPIO_MODE_INPUT);
		gpio_set_pull_mode(Entrada[j], GPIO_PULLDOWN_ONLY);
	}
	
	//if (gpio_get_level(PULSADOR1) == 1){}
	
   //Bucle infinito 
    while(1) {
		//printf("Escribo en serial\n");
		
        for(int8_t k= 0; k < Cantidad_Entradas; k++){

            if(gpio_get_level(Entrada[k]) == 1){ //si el pulsador esta siendo pulsado
                    contador[k] += 1; //incremento el contador cada 1 milisegundo (sabiendo que la ejecucion de cada instruccion dentro del while no causa casi ningun retardo)
                    gpio_set_level(Salida[k], 0); //apago led
                }else{ //si no esta pulsado
                    if(contador[k]>0){ //verifico que el contador sea mayor a cero 
                        gpio_set_level(Salida[k], 1); //prendo led
                        contador[k] -= 1; //decremento el contador cada 1 milisegundo (sabiendo que la ejecucion de cada instruccion dentro del while no causa casi ningun retardo)
                    }else{
                        gpio_set_level(Salida[k], 0); //apago led 
                    }    
                }
	    }

        vTaskDelay(1 / portTICK_PERIOD_MS); //Espero 1 milisegundo
		
		
    }

}