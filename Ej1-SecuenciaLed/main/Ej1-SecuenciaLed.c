/*=============================================================================
 * Author: Fabian Burgos
 * Date: 07/04/2021 
 * Board: ESP32-CAM
 *===========================================================================*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "soc/soc.h" //disable brownout detector
#include "soc/rtc_cntl_reg.h" //disable brownout detector (deteccion de apagon)
#include "soc/rtc_wdt.h"

#define Cantidad_LED 		6
#define PULSADOR1       	16
#define PULSADOR2       	0
#define Tiempo_prendido		500

int8_t led [Cantidad_LED] = {4, 2, 14, 15, 13, 12};

void app_main()
{
	//Configuración
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
    rtc_wdt_protect_off();
    rtc_wdt_disable();
	for(int8_t i = 0; i < Cantidad_LED; i++){
		gpio_pad_select_gpio(led[i]);
		gpio_set_direction(led[i], GPIO_MODE_OUTPUT);
        gpio_set_level(led[i], 0);
	}
	
	gpio_pad_select_gpio(PULSADOR1);
	gpio_set_direction(PULSADOR1, GPIO_MODE_INPUT);
	gpio_set_pull_mode(PULSADOR1, GPIO_PULLDOWN_ONLY);
    gpio_pad_select_gpio(PULSADOR2);
	gpio_set_direction(PULSADOR2, GPIO_MODE_INPUT);
	gpio_set_pull_mode(PULSADOR2, GPIO_PULLDOWN_ONLY);

   int8_t i = 0, sentido = 0;

   //Bucle infinito
   while( true )
    {
        
        //Realizo for para esperar el tiempo establecido pero para poder ejecutar en un tiempo muy corto 
        //la detccion del cambio de estado de los pulsadores, ESTO PERMITE PRECTICAMENTE
        //cambiar el sentido de la secuencia en cualquier momento de la ejecución
        for(int8_t j=0; j<100; j++){
            
            vTaskDelay((Tiempo_prendido/100)/portTICK_PERIOD_MS);

            //cambio de sentido
            if (gpio_get_level(PULSADOR1) == 1)
		    {
			    sentido = 1;
                printf("Cambio a sentido 1\n");
            }else if (gpio_get_level(PULSADOR2) == 1)
            {
                sentido = 0;
                printf("Cambio a sentido 0\n");
            }	

        }
		
        printf("Realizo secuencia\n");
        //realizo secuencia
	   	if (sentido == 0)
		{
			gpio_set_level(led[i], 0);
			i++;
			if ( i >= Cantidad_LED ){ i = 0; }
			gpio_set_level(led[i], 1);
		}
	   	else 
		{
			gpio_set_level(led[i], 0);
			i--;
			if ( i < 0 ){
				i = Cantidad_LED;
			}
			gpio_set_level(led[i], 1);
		}
			
	}
   return 0;
}