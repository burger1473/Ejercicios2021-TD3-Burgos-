#ifndef PULSADOR_H_
#define PULSADOR_H_

/*==================[ Inclusiones ]============================================*/
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "freertos/semphr.h"

/*==================[ Ficheros ]============================================*/
#include "global.h"

/*==================[ Definiciones ]============================================*/
#define ESP_INTR_FLAG_DEFAULT 0

//========================== Funciones ====================================
void config_pulsador_int(void);

//===================== Variables ====================================
extern gpio_int_type_t Pulsador_Incremento[];
extern gpio_int_type_t Pulsador_Decremento[];

#endif