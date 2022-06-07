#ifndef JOYSTICK_H // Caso JOYSTICK_H não exista entra no if
#define JOYSTICK_H // Cria o JOYSTICK_H
#include "mbed.h" // Importa Biblioteca do mbed

class Joystick { // Inicialização da Classe JoyStick
    public:
        Joystick(PinName xPin, PinName yPin, PinName butPin, PinName zMais, PinName zMenos); // x, y e botão do JoyStick
        
        //Métodos:
        int GetXValue(); // Inicialização do Método para Pegar o Valor do X
        int GetYValue(); // Inicialização do Método para Pegar o Valor do Y
        int GetZValue(); // Inicialização do Método para Pegar o Valor do Z
        int Button(); // Inicialização do Método para Receber Input do Botão
        
        // GPIO's:
        AnalogIn xAxis; // Inicialização do AnalogIn do Eixo X
        AnalogIn yAxis; // Inicialização do AnalogIn do Eixo Y
        DigitalIn Sw; // Inicialização do DigitalIn do Botão
        DigitalIn Mais; // Inicialização do Botão para Subir o Z | z+
        DigitalIn Menos; // Inicialização do Botão para Descer o Z | z-
        
        // Variáveis:
        int offset; // Inicaliza a Variável de Compensação do JS
        int xValue, yValue; // Inicialização dos Valores dos Eixos
};

#endif // Final do if