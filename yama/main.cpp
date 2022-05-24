#include "mbed.h" // Importa a Biblioteca do mbed
#include "MotorPasso.h" // Importa Arquivo da Classe Motor de Passo
#include "Joystick.h"

MP MPx(D2, D3, D4, D5, PA_5, PA_6); // Bobina1, Bobina2, Bobina3, Bobina4, FimDeCursoInicial, FimDeCursoFinal | x
//MP MPy(D6, D7, D8, D9, PC_7, PA_9); // Bobina1, Bobina2, Bobina3, Bobina4, FimDeCursoInicial, FimDeCursoFinal | y
//MP MPz(D10, D11, D12, D13, PB_4, PB_5); // Bobina1, Bobina2, Bobina3, Bobina4, FimDeCursoInicial, FimDeCursoFinal | z

//Joystick JS(); // x, y, botão | joystick

int main(){ // Loop Principal
    MPx.MotorReferenciamento(0);
}
