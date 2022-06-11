#include "MotorPasso.h" // Importa Arquivo de Classe do Motor de Passo

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
        
    // Variáveis da Classe
    this -> espera = 3; // Tempo de Espera para Mover um Passo [ms]
    this -> QntPassos = 0; 
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
void MP::MotorReferenciamento(bool Dir) {
    while(FCi == 1 & FCf == 1) { // Checa se Ambos os Fins de Curso não estão pressionados
        MoverMotor(Dir); // Move o Motor para o Fim de Curso
    }
    
    while(FCi == 0 | FCf == 0) { // Checa se Apenas um Fim de Curso está pressionado
        MoverMotor(!Dir); // Move o Motor para o Outro Lado
    }
    
    while(FCi == 1 & FCf == 1) { // Checa se Ambos os Fins de Curso não estão pressionados
        MoverMotor(Dir); // Move o Motor para o Fim de Curso
    }
}

int MP::MotorTamanhoFuso(int Dir) {
    int passosFuso = 0;
    while(FCf == 1) {
        MoverMotor(Dir);
        passosFuso++;
    }
    return passosFuso;
}

 // Método da Aceleração do Motor
void accMotor() {   
}

 // Método para Mover Motor com Input de Passos e Direção 
void MP::MotorPorPasso(int passos, int Dir) {
    int p = 0; // Inicia Contagem de Passos
    while(p < passos) { // Move o Motor Enquando não Atingir a Quantidade de Passos
        MoverMotor(Dir); // Move o Motor
        p++;
    }
}

 // Método para Mover o Motor com Coordenadas
void MP::MotorDosagem(int Delta) {
    if(Delta < 0) { // Se a Coordenada der Negativa o Motor vai para -Eixo
        MotorPorPasso(abs(Delta), 0); // Move o Motor com Coord passos, para - Eixo
    }
    
    else { // Se a Coordenada der Positiva o Motor vai para +Eixo
        MotorPorPasso(Delta, 1); // Move o Motor com Coord passos, para +Eixo
    }
}
