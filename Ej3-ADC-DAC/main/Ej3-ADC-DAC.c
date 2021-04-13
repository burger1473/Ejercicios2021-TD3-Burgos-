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
#include <driver/adc.h>
#include <driver/dac.h>

void app_main() {
	//Configuración
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
    rtc_wdt_protect_off();
    rtc_wdt_disable();
	
    //Configuro pin 4 como salida
    gpio_pad_select_gpio(33);
	gpio_set_direction(33, GPIO_MODE_OUTPUT);
    gpio_set_level(33, 0);

	//Configuro ADC
    adc1_config_width(ADC_WIDTH_12Bit); //configuro la resoluncion del adc1
    adc1_config_channel_atten (ADC1_CHANNEL_0 ,ADC_ATTEN_11db); //Canal 1 para leer potenciometro
    adc2_config_channel_atten (ADC2_CHANNEL_0, ADC_ATTEN_11db); //Canal 2 para leer tension generada con el DAC

    //Configuro DAC
    dac_output_enable(DAC_CHANNEL_1); 
	
   //Bucle infinito 
    while(1) {

		int lectura_pote = adc1_get_raw(ADC1_CHANNEL_0);
        printf("El valor del ADC1 es %d\n", lectura_pote);
        //Primera parte del ejercicio
        if(lectura_pote>125){gpio_set_level(33, 1);}else{gpio_set_level(33, 0);}

        //Segunda parte del ejercicio 
        dac_output_voltage(DAC_CHANNEL_1,(lectura_pote*255/4095));  //Convierto Resolucion ADC a DAC
                                                               //0 y 255 DAC -- 8 bit -- va de 0 V a VDA (pin de alimentacion analogico) 
                                                               //0 y 4095 ADC

        int lectura_DAC = 0;
        adc2_get_raw(ADC2_CHANNEL_0, ADC_WIDTH_12Bit, &lectura_DAC);
        printf("El valor del ADC2 es %d\n", lectura_DAC);

        vTaskDelay(500/portTICK_PERIOD_MS);   
		
    }

}