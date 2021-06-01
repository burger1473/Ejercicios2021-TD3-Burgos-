/*==================[ Inclusiones ]============================================*/
#include "../include/led.h"

/*==================[Implementaciones]=================================*/
void inicializarLed(uint8_t pin){
    gpio_pad_select_gpio(pin);
    gpio_set_direction(pin , GPIO_MODE_OUTPUT);
    gpio_set_level(pin, 0);
}

void PrenderLed(uint8_t pin){
    gpio_set_level(pin, 1);
}

void ApagarLed(uint8_t pin){
    gpio_set_level(pin, 0);
}