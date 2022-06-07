#include "Joystick.h" // Importa Arquivo de Classe do Motor de Passo
//Serial pc(USBTX, USBRX);

Joystick::Joystick( // __init__
    PinName xPin, // Pino do x
    PinName yPin, // Pino do y
    PinName butPin, // Pino do Botão do JS
    PinName zMais,
    PinName zMenos
) : 
    xAxis(xPin), // Medição do Joystick em x
    yAxis(yPin), // Medição do Joystick em y
    Sw(butPin), // Botão do Joystick
    Mais(zMais), // Botão para Subir o Z
    Menos(zMenos) { // Botão para Descer o Z

    // Variáveis de Classe:
    this -> offset = 200; // Compensação para Erro do JS
}

 // Método para Pegar o Valor do X
int Joystick::GetXValue() { // 
    int xSum = 0; // Soma dos Pontos de X
    
    // Soma 5 Medições no Ponto X
    for(int i = 0; i < 5; i++) {
        xSum += xAxis.read() * 1000;
    }
    
    int xValue = xSum/5; // Faz a Média das 5 Medições
    
    // Checa Valores do X
    if(xValue < 0) {
        return 0; // x = 0
    }
    else if(xValue > 1000) {
        return 1000; // x = 1000
    }
    else {
        return xValue; // x = xValue
    }
}

 // Método para Pegar o Valor do Y
int Joystick::GetYValue() {
    int ySum = 0; // Soma dos Pontos de Y
    
    // Soma 5 Medições no Ponto Y
    for(int i = 0; i < 5; i++) {
        ySum += yAxis.read() * 1000;
    }
    
    int yValue = ySum/5; // Faz a Média das 5 Medições
    
    // Checa Valores do Y
    if(yValue < 0) {
        return 0; // y = 0
    }
    else if(yValue > 1000) {
        return 1000; // y = 1000
    }
    else {
        return yValue; // y = xValue
    }
}

 // Método para Pegar o Valor do Z
int Joystick::GetZValue() {
    
}

 // Método para Receber Input do Botão
int Joystick::Button() {
    if(Sw == 0) {
        return 1;
    }
    else {
        return 0;
    }
//    return estado;
}

















