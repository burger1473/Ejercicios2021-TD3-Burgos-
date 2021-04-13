/*=============================================================================
 * Author: Fabian Burgos
 * Date: 07/04/2021 
 * Board: ESP32-CAM 
 * Pin 4 led red
 * Pin 2 led amarillo
 * Pin 14 led verde
 *===========================================================================*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "soc/soc.h" //disable brownout detector
#include "soc/rtc_cntl_reg.h" //disable brownout detector (deteccion de apagon)
#include "soc/rtc_wdt.h"
#include "../include/semaforo.h"


void app_main() {
	//Configuración
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
    rtc_wdt_protect_off();
    rtc_wdt_disable();

    Semaforo_init(); //inicio pines del semaforo
	
	
   //Bucle infinito 
    while(1) {
		printf("Escribo en serial2\n");
		
        Semaforo_siclo(); //hago un siclo del semaforo
        
    }

}