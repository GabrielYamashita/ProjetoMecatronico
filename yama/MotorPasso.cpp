#include "MotorPasso.h" // Importa Arquivo de Classe do Motor de Passo
Serial pc(USBTX, USBRX);

MP::MP( // __init__
    PinName a, // Pino A
    PinName b, // Pino B
    PinName c, // Pino C
    PinName d, // Pino D
    PinName fci, // Pino do Fim de Curso Inicial
    PinName fcf // Pino do Fim de Curso Final
) : 
        MotorPasso(a, b, c, d), // BusOut do Motor de Passo
        FCi(fci), // Fim de Curso Inicial
        FCf(fcf) { // Fim de Curso Final
    this -> revCompleta = 200; // Passos para dar uma Revolução Completa
    this -> espera = 2; // Tempo de Espera para Mover um Passo [ms]
}

 // Método para Mover Motor Infinitamente com Input de Direção
void MP::MoverMotor(int Dir) {
    if(Dir == 0) { // Gira no Sentido Horário
        this->Passo--;
        
        if(Passo == -1){this->Passo = 3;}
        
        MotorPasso = 1 << Passo;
        wait_ms(espera);
        
    } else if(Dir == 1) { // Gira no Sentido Anti-Horário
        this -> Passo++;
        
        if(Passo == 4){this -> Passo = 0;}
        
        MotorPasso = 1 << Passo; // Muda o Bitwise
        wait_ms(espera); // Espera o Tempo Pré-Definido
    }
}

 // Método para Mover Motor para o Referenciamento
void MP::MotorReferenciamento(void) {
    while(FCi == 1 & FCf == 1) {
        MoverMotor(0);
    }
    
    while(FCi == 0 | FCf == 0) {
        MoverMotor(1);
    }
    
    while(FCi == 1 & FCf == 1) {
        MoverMotor(0);
    }
}

void velocidadeMotor() {   
}

 // Método para Mover Motor com Input de Passos e Direção 
void MP::MotorPorPasso(int Dir, int passos) {
    int p = 0;
    while(p < passos) {
        MoverMotor(Dir);
        p++;
    }
}






