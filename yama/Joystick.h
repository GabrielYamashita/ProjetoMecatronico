#ifndef JOYSTICK_H // Caso JOYSTICK_H não exista entra no if
#define JOYSTICK_H // Cria o JOYSTICK_H
#include "mbed.h" // Importa Biblioteca do mbed

class Joystick { // Inicialização da Classe JoyStick
    public:
        Joystick(PinName x, PinName y, PinName but); // x, y e botão do JoyStick
        
        //Métodos:
        int Coordenada();
        void Button();
        
        // GPIO's:
        AnalogIn xAxis;
        AnalogIn yAxis;
        DigitalIn Sw;
        
        // Variáveis:
        int lim;
        int x, y;
};

#endif // Final do if