#ifndef JOYSTICK_H // Caso JOYSTICK_H não exista entra no if
#define JOYSTICK_H // Cria o JOYSTICK_H
#include "mbed.h" // Importa Biblioteca do mbed

class Joystick { // Inicialização da Classe JoyStick
    public:
        Joystick(PinName x, Piname y, PinName but); // x, y e botão do JoyStick
        
        //Métodos:
        
        // GPIO's:
        AnalogIn xAxis;
        AnalogIn yAxis;
        DigitalIn Sw;
        
        // Variáveis
        int x;
        int y;      
};

#endif // Final do if