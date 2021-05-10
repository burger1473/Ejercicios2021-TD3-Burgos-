#include "../include/Global.h"
#include "../include/Led.h"

// Funcion para iniciar los leds como salida
// Es con un bucle for para iniciar = cantidad de leds como de participantes
void iniciarLed (){
	for(uint8_t i = 0; i < Cantidad_Participantes; i++){
			gpio_pad_select_gpio(Salida[i]);
			gpio_set_direction(Salida[i], GPIO_MODE_OUTPUT);
			gpio_set_level(Salida[i], 0);
		}
}

void PrenderLed(uint8_t i){
		gpio_set_level(i, 1); // Prendo led
}

void ApagarLed(uint8_t i){
 		gpio_set_level(i, 0); // Apago led
}