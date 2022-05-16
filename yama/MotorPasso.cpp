#include "MotorPasso.h" // Importa Arquivo de Classe do Motor de Passo

MP::MP(PinName a, PinName b, PinName c, PinName d, PinName fci, PinName fcf) : MotorPasso(a, b, c, d), FCi(fci), FCf(fcf) { // __init__ , cria o BusOut, FCi e FCf a partir dos PinNames
    this -> revCompleta = 200; // Passos para dar uma Revolução Completa
    this -> espera = 2; // Tempo de Espera para Mover um Passo [ms]
}

void MP::MoverMotor(int Dir) { // Método para Mover Motor Infinitamente com Input de Direção
    if(FCi == 1 & FCf == 1) { // Checa se os Fim de Cursos estão Pressionados
        if(Dir == 0) { // Gira no Sentido Horário
            for(int i = 3; i > -1; i--) { // Loop para Alternar as Bobinas
                MotorPasso = 1 << i; // Muda o Bitwise
                wait_ms(espera); // Espera o Tempo Pré-Definido
            }
        } else if(Dir == 1) { // Gira no Sentido Horário
            for(int i = 0; i < 4; i++) { // Loop para Alternar as Bobinas
                MotorPasso = 1 << i; // Muda o Bitwise
                wait_ms(espera); // Espera o Tempo Pré-Definido
            }
        }
    }
}

void MP::MotorReferenciamento(void) { // Método para Mover Motor para o Referenciamento
    if(FCi == 1 & FCf == 1) { // Checa se os Fim de Cursos estão Pressionados
        for(int i = 3; i > -1; i--) { // Loop para Alternar as Bobinas
            MotorPasso = 1 << i; // Muda o Bitwise
            wait_ms(espera); // Espera o Tempo Pré-Definido
        }
    }
}

void MP::MotorPorPasso(int passos, int Dir) { // Método para Mover Motor com quantidade de Passos e Direção Pré-Definida 
    if(FCi == 1 & FCf == 1) {
        for(int passosMotor = 0; passosMotor < passos/4; passosMotor++) { // Loop para Contar os Passos Realizados
            if(Dir == 0) { // Gira no Sentido Horário
                for(int i = 3; i > -1; i--) { // Loop para Alternar as Bobinas
                    MotorPasso = 1 << i; // Muda o Bitwise
                    wait_ms(espera); // Espera o Tempo Pré-Definido
                }
            } else if(Dir == 1) { // Gira no Sentido Horário
                for(int i = 0; i < 4; i++) { // Loop para Alternar as Bobinas
                    MotorPasso = 1 << i; // Muda o Bitwise
                    wait_ms(espera); // Espera o Tempo Pré-Definido
                }
            }
        }
    }
}






