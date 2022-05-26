#include "mbed.h" // Importa a Biblioteca do mbed
#include "MotorPasso.h" // Importa Arquivo da Classe Motor de Passo
#include "Joystick.h"

MP MPx(D2, D3, D4, D5, PA_5, PA_6); // Bobina1, Bobina2, Bobina3, Bobina4, FimDeCursoInicial, FimDeCursoFinal | x
MP MPy(D6, D7, D8, D9, PC_7, PA_9); // Bobina1, Bobina2, Bobina3, Bobina4, FimDeCursoInicial, FimDeCursoFinal | y
MP MPz(D10, D11, D12, D13, PB_4, PB_5); // Bobina1, Bobina2, Bobina3, Bobina4, FimDeCursoInicial, FimDeCursoFinal | z

Joystick JS(A1, A2, A3); // x, y, botão | joystick

int estado = 0;

int main(){ // Loop Principal
    while(estado == 0) { // MENU
        printf("Menu");
    } // FIM DO ESTADO DE MENU
    
    
    
    while(estado == 1) {
        printf("Referenciamento");
        MPx.MotorReferenciamento(0);
        MPy.MotorReferenciamento(0);
        MPz.MotorReferenciamento(0);
        
    } // FIM DO ESTADO DE REFERENCIAMENTO
    
    
    
    while(estado == 2) { // POSICIONAMENTO
        printf("Posicionamento");
            while(1) {
            int x, y;
            if(JS.Coordenada == 0) {
                MPx.MoveMotor(1);
                x--;
            }
            if(JS.Coordenada == 1) {
                MPx.MoveMotor(0);
                x++;
            }
            if(JS.Coordenada == 2) {
                MPy.MoveMotor(1);
                y--;
            }
            if(JS.Coordenada == 3) {
                MPy.MoveMotor(0);
                y++;
            }
        }
    } // FIM DO ESTADO DE POSICIONAMENTO
    
    
    
    while(estado == 3) { // DOSAGEM
        printf("Dosagem");
    } // FIM DO ESTADO DE DOSAGEM
    
    
    
    while(estado == 4) { // EMERGÊNCIA
        printf("Emergência");
    } // FIM DO ESTADO DE EMEREGÊNCIA
}
