#ifndef MOTORPASSO_H // Caso MOTORPASSO_H não exista entra no if
#define MOTORPASSO_H // Cria o MOTORPASSO_H
#include "mbed.h" // Importa Biblioteca do mbed

class MP { // Inicialização da Classe MP (Motor de Passo)
    public:
        MP(PinName a, PinName b, PinName c, PinName d, PinName fci, PinName fcf); // Bobina1, Bobina2, Bobina3, Bobina4, FimDeCursoInicial, FimDeCursoFinal | x
        
        // Métodos:
        void MoverMotor(int Dir); // Inicialização do Método para Mover Motor Infinitamente com Input de Direção
        void MotorReferenciamento(bool D); // Inicialização do Método para Mover Motor para o Referenciamento
        void MotorPorPasso(int passos, int Dir); // Inicialização do Método para Mover Motor com quantidade de Passos e Direção Pré-Definida
        void MotorDosagem(int Coord); // Inicialização do Método para Mover o Motor com Coordenadas na Rotina da Dosagem

        // GPIO's:
        BusOut MotorPasso; // Inicialização do BusOut MotorPasso
        DigitalIn FCi; // Inicialização do DigitalIn Fim de Curso Inicial
        DigitalIn FCf; // Inicialização do DigitalIn Fim de Curso Final
        
        // Variáveis:
        int revCompleta; // Declaração da Variável Rev. Completa do Motor
        int espera; // Declaração da Variável Espera do Tempo entre Passos
        int Passo; // Declaração
};

#endif // Final do if