#include "../include/Global.h"
#include "../include/Led.h"
#include "../include/Pulsador.h"

// Tipo de datos para la variable de estados
typedef enum{
    Led_Apagado,
    Led_Encendido
} estadoMEF_t;

// Variable de estado (global)
// Arreglo para tener tantas MEF = Cantidad de participantes
estadoMEF_t estadoActual[Cantidad_Participantes];

// Declaramos un contador para determinar el tiempo an alto de cada pulsador
// Hay cantidad de contadores = cantidad de participantes
int contador[Cantidad_Participantes];

// Funcion para iniciar los pulsadores como entrada, con resistencia pull down
// Es con un bucle for para iniciar = cantidad de pulsadores como de participantes
void iniciarPulsadores (){

for(uint8_t i = 0; i < Cantidad_Participantes; i++){
		gpio_pad_select_gpio(Entrada[i]);
		gpio_set_direction(Entrada[i], GPIO_MODE_INPUT);
        gpio_set_pull_mode(Entrada[i], GPIO_PULLDOWN_ONLY);

	}
}

// Funcion para iniciar los contadores todos en un valor = 0
// Es con un bucle for para iniciar = cantidad de contadores como de participantes
void iniciarContadores (){

for(uint8_t i = 0; i < Cantidad_Participantes; i++){
     contador[i] = 0;
	}
}   

// Funcion para iniciar MEF, con el estado actual igual a Led_Apagado
// Es con un bucle for para iniciar = cantidad de MEF como de participantes
void InicializarMEF(){
    for(uint8_t i = 0; i < Cantidad_Participantes; i++){
			 estadoActual[i] = Led_Apagado;
	}

}

// Funcion para actualizar Pulsadores
// Es con un bucle for para actualizar la MEf = cantidad de participantes
void actualizarPulsador()
{   
    // En cada ciclo se Actualiza la MEF de cada participante
    for(uint8_t i = 0; i < Cantidad_Participantes; i++){

        // Los posibles casos son:  Led_Apagado
        //                          Led_Encendido


	switch (estadoActual[i]) {
		case Led_Apagado:       // Si el led esta apagado
        {     

                if(gpio_get_level(Entrada[i]) == 1){ // Si el pulsador esta siendo pulsado
                    contador[i] += 1;                // Incremento el contador cada 5 milisegundo (sabiendo que la ejecucion de cada instruccion dentro del while no causa casi ningun retardo)
                }else if(contador[i]>0){             // Si el pulsador no esta siendo pulsado, verifico que el contador sea mayor a cero 
                    estadoActual[i] = Led_Encendido; // Cambio de estado de la MEF
                       
                    }    
		}
		break;
		case Led_Encendido:{                        // Si el led esta encendido
			if(contador[i]>0){                      // Verifico que el contador sea mayor a cero                        

             PrenderLed(Salida[i]);                 // Prendo led
             contador[i] -= 1;                      // Decremento el contador cada 5 milisegundo (sabiendo que la ejecucion de cada instruccion dentro del while no causa casi ningun retardo)
             } else{    
                 ApagarLed(Salida[i]);              // Apago led
                 estadoActual[i] = Led_Apagado;     // Cambio de estado de la MEF
            
            }
		break;
		
		default:{
			//Si cae en un estado no valido, reinicio
			InicializarMEF();
		}
		break;
    }

    }


		
}
}