#include "Joystick.h" // Importa Arquivo de Classe do Motor de Passo
Serial pc(USBTX, USBRX);

Joystick::Joystick( // __init__
    PinName x, // Pino do x
    PinName y, // Pino do y
    PinName but // Pino do Botão
) : 
        xAxis(x), // Medição do Joystick em x
        yAxis(y), // Medição do Joystick em y
        Sw(but) { // Botão do Joystick

        // Variáveis de Classe:
        this -> lim = 500;
}

int Joystick::Coordenada() {
    int a;
    x = xAxis.read() * 1000; // Coordenada do x em int
    y = yAxis.read() * 1000; // Coordenada do y em int
    
    if (x < 500 - lim) { // -x
        a = 0;
    }
    
    if (x > 500 + lim) { // +x
        a = 1;
    }
    
    if (y < 500 - lim) { // -y
        a = 2;
    }
    
    if (y > 500 + lim) { // +y
        a = 3;
    }
    return a;
}

void Joystick::Button() {
}

















