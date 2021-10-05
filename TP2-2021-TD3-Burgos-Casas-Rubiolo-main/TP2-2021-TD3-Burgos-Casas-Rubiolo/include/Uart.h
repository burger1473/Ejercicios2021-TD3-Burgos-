#ifndef UART_H_
#define UART_H_
/*==================[ Inclusiones ]============================================*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <stdio.h>
#include "driver/uart.h"
#include "sdkconfig.h"
#include "freertos/queue.h" //incluyo la libreria para usar cola
#include "global.h"

/*==================[ Variables ]===================================*/
extern uint8_t contador[];

QueueHandle_t cola[N_LED];

/*==================[ Definiciones ]===================================*/

#define PIN_TX (UART_PIN_NO_CHANGE)
#define PIN_RX (UART_PIN_NO_CHANGE)
#define PIN_RTS (UART_PIN_NO_CHANGE)
#define PIN_CTS (UART_PIN_NO_CHANGE)
#define UART_PORT_NUM     (UART_NUM_0)
#define UART_BAUD_RATE     115200
#define BUF_SIZE (1024)

/*==================[ Prototipos ]===================================*/
void config_Uart(void);

#endif