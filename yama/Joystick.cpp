#include "MotorPasso.h" // Importa Arquivo de Classe do Motor de Passo
Serial pc(USBTX, USBRX);

MP::MP( // __init__
    PinName x, // Pino do x
    PinName y, // Pino do y
    PinName but // Pino do Botão
) : 
        xAxis(x), // Medição do Joystick em x
        yAxis(y), // Medição do Joystick em y
        Sw(but) // Botão do Joystick
        
    // Variáveis da Classe
    this -> x = xAxis.read() * 1000; // Coordenada do x em int
    this -> y = yAxis.read() * 1000; // Coordenada do y em int
}
