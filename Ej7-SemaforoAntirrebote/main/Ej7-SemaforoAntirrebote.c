/*=============================================================================
 * Author: Fabian Burgos
 * Date: 07/04/2021 
 * Board: ESP32-CAM 
 * Pin 4 led red
 * Pin 2 led amarillo
 * Pin 14 led verde
 *===========================================================================*/
int Modo=0;

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "soc/soc.h" //disable brownout detector
#include "soc/rtc_cntl_reg.h" //disable brownout detector (deteccion de apagon)
#include "soc/rtc_wdt.h"
#include "../include/semaforo.h"
#include "../include/pulsador.h"

#define PULSADOR 16



void app_main() {
	//Configuración
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
    rtc_wdt_protect_off();
    rtc_wdt_disable();

    Pulsador_init(PULSADOR);
    Semaforo_init(); //inicio pines del semaforo
	
	
   //Bucle infinito 
    while(1) {
		//printf("Escribo en serial3\n");
		
        Actualizar_estado(PULSADOR);

        if(Modo){
            Semaforo_siclo(); //hago un siclo intermitente
        }else{
            Semaforo_intermitente(); //hago un siclo intermitente
        }
        
        
       // vTaskDelay(10/ portTICK_PERIOD_MS); //Espero 10 milisegundo
    }

}