#include "Uart.h"

//========================== Prototipos =====================================
static void tareaEco(void *arg);

//===================== Funcion principal ====================================
void config_Uart(void)
{
    xTaskCreatePinnedToCore(tareaEco, "uart_tareaEco", configMINIMAL_STACK_SIZE*3, NULL, tskIDLE_PRIORITY+1, NULL, PROCESADORB);

    for(int h=0; h<N_LED; h++){
        cola[h]= xQueueCreate( 3 , sizeof(int8_t) ); // Creación de la cola
    }
}

//========================== Variables ====================================
 int8_t menos_1 = -1;
 int8_t mas_1 = 1;

//========================== Funciones ====================================
static void tareaEco(void *arg)
{
    // Parámetros de configuración para la UART
    uart_config_t uartConfig = {
        .baud_rate = UART_BAUD_RATE,              
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    // Configura la UART con los parámetros anteriores    
    ESP_ERROR_CHECK(uart_param_config(UART_PORT_NUM, &uartConfig));

    // Configura los pines de la UART (TX: IO17 (UART2 default), RX: IO16 (UART2 default), RTS: no usado, CTS: no usado)
    ESP_ERROR_CHECK(uart_set_pin(UART_PORT_NUM, PIN_TX, PIN_RX, PIN_RTS, PIN_CTS));

    //Instala el controlador UART y establece la UART en la configuración predeterminada.
    //El controlador UART ISR se adjuntará al mismo núcleo de CPU en el que se ejecuta esta función.
      
    ESP_ERROR_CHECK(uart_driver_install(
                        UART_PORT_NUM,     // Número de puerto
                        BUF_SIZE * 2,   // buffer rx 
                        0,              // buffer TX -- 0 no usa, la función de transmisión bloqueará la tarea hasta que se envién los datos
                        0,              // tamaño de la cola UART
                        NULL,           // handle de la cola UART
                        0               // flag para interrupciones
                    ));
    
    uint8_t data[BUF_SIZE]; // configura un buffer para los datos entrantes

    while (true) 
    {
        int len = uart_read_bytes(UART_PORT_NUM, data, BUF_SIZE, 20 / portTICK_RATE_MS); //lee desde la UART
        vTaskDelay(1000/portTICK_PERIOD_MS);
        
        if(len==5){     // si recibo 5 datos
            if(data[2]==47 && data[3]==47 && data[4]==47 ){ // ASCII 47 = /

                if(data[1]==65){            // ASCII 65 = A
                    if(data[0]==43){            // ASCII 43 = +
                        //contador[0]=contador[0]+1;
                        xQueueSend( cola[0] , &mas_1,  portMAX_DELAY ); // Envío dato a la cola con tiempo de retardo infinito, es decir, cuando la cola esta llena se bloquea hasta que se libere un espacio

                    }
                    if(data[0]== 45 ){            // ASCII 45 = -
                        //contador[0]=contador[0]-1;
                        xQueueSend( cola[0] , &menos_1,  portMAX_DELAY ); // Envío dato a la cola con tiempo de retardo infinito, es decir, cuando la cola esta llena se bloquea hasta que se libere un espacio

                    }
                }
                if(data[1]==66){            // ASCII 66 = B
                    if(data[0]==43){            // ASCII 43 = +
                        //contador[1]=contador[1]+1;
                        xQueueSend( cola[1] , &mas_1,  portMAX_DELAY ); // Envío dato a la cola con tiempo de retardo infinito, es decir, cuando la cola esta llena se bloquea hasta que se libere un espacio

                    }
                    if(data[0]== 45 ){            // ASCII 45 = -
                        //contador[1]=contador[1]-1;
                        xQueueSend( cola[1] , &menos_1,  portMAX_DELAY ); // Envío dato a la cola con tiempo de retardo infinito, es decir, cuando la cola esta llena se bloquea hasta que se libere un espacio

                    }
                }
            }
        }

    }
}