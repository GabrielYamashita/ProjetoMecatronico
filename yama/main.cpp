// BIBLIOTECAS:
#include "mbed.h" // Importa a Biblioteca do mbed
#include "MotorPasso.h" // Importa Arquivo da Classe do Motor de Passo
#include "Joystick.h" // Importa Aqrquivo da Classe do Joystick
#include "TelasLCD.h" // Importa as Funções das Telas do LCD

// OBJETOS DE CLASSE PARA A MÁQUINA:
MP MPx(D2, D3, D4, D5, PA_5, PA_6); // Bobina1, Bobina2, Bobina3, Bobina4, FimDeCursoInicial, FimDeCursoFinal | x
MP MPy(D6, D7, D8, D9, PC_7, PA_9); // Bobina1, Bobina2, Bobina3, Bobina4, FimDeCursoInicial, FimDeCursoFinal | y
//MP MPz(D10, D11, D12, D13, PB_4, PB_5); // Bobina1, Bobina2, Bobina3, Bobina4, FimDeCursoInicial, FimDeCursoFinal | z

Joystick JS(A1, A2, A3); // x, y, botão | joystick


// GPIO's:
InterruptIn Emerg(D12); // Botão de Emergência


// VARIÁVEIS:
int xAtual; // Coord x Atual do Motor | x
int yAtual; // Coord x Atual do Motor | y
int zAtual; // Coord x Atual do Motor | z

int X[] = {}; // Lista das Coordenadas dos Recipientes | x
int Y[] = {}; // Lista das Coordenadas dos Recipientes | y
int Z[] = {}; // Lista das Coordenadas dos Recipientes | z
int V[] = {0}; // Lista dos Volumes dos Recipientes | V


// INICIALIZAÇÃO DE FUNÇÕES:
void etapaEmerg(void); // Inicialização da Função do Estado de Emergência | Emergência
void JS_Posicionamento(int x, int y);
void pulsoPipeta(int z); // Inicialização da Função para Mover e Emitir Pulso da Pipeta | Dosagem


// ESTADOS DE MÁQUINA:
int estado = 0;


// LOOP PRINCIPAL:
int main(){
    Emerg.fall(&etapaEmerg); // Aciona o Estado de Emergência

    while(estado == 1) { // MENU
        printf("\rMENU\n");
        
//        estado = 1; // Vai para a Rotina de Referênciamento | IHM
    } // FIM DO ESTADO DE MENU
    
    
    
    while(estado == 1) { // REFERÊNCIAMENTO
        printf("/rREFERENCIAMENTO/n");
        MPx.MotorReferenciamento(0); // Realiza o Referenciamento pro 0 do eixo X | x
        MPy.MotorReferenciamento(0); // Realiza o Referenciamento pro 0 do eixo Y | y
//        MPz.MotorReferenciamento(0); // Realiza o Referenciamento pro 0 do eixo Z | z
        
//        estado = 2; // Vai para a Rotina de Posicionamento | IHM
    } // FIM DO ESTADO DE REFERENCIAMENT
    
    
    
    while(estado == 2) { // POSICIONAMENTO
        // Variáveis de Processos
        int Pega = 0; // processo do pega
        bool Processo = true; // processo dos soltas
        
        xAtual = JS.GetXValue(); // Pega a Coordenada do x Atual | JS
        yAtual = JS.GetYValue(); // Pega a Coordenada do y Atual | JS
        
        JS_Posicionamento(xAtual, yAtual); // Move os Motores de Acordo com o JS
        
        // Ponto de Coleta/Pega:
        while(Pega == 0) { // Checa se Foi Definido o Ponto de Pega
            Pega = 1; // Condição de Saída | IHM
        }
        
        // Pontos de Tranferência/Soltas:
        while(Processo) { // Checa se Foram Definidos os Pontos de Solta
            Processo = false; // Condição de Saída | IHM
        }
    
//        estado = 3; // Vai para a Rotina de Dosagem | IHM
    } // FIM DO ESTADO DE POSICIONAMENTO
    
    
    
    while(estado == 3) { // DOSAGEM
        // Define as Coordenadas do Pega
        int xPega = X[0];
        int yPega = Y[0];
        int zPega = Z[0];

        // Move o Motor para as Coordenadas do Pega
        MPx.MotorPorPasso(xPega, 1);
        MPy.MotorPorPasso(yPega, 1);
        pulsoPipeta(zPega); // Pega o Líquido do Pega para a Pipeta
        
        int recip = 1; // Começa no Índice 1 da Lista, dos Recipientes/Soltas
        while(recip < sizeof(X)/sizeof(X[0])) { // Roda com o Tamanho da lista dos Recipientes
            
            // Define Dados do Recepiente
            int xAlvo = X[recip]; // Coord Recipiente X
            int yAlvo = Y[recip]; // Coord Recipiente Y
            int zAlvo = Z[recip]; // Coord Recipiente Z
            int vAlvo = V[recip]; // Volume do Recipiente
            
            int volRecip = 0;
            while(volRecip < vAlvo) { // Loop do Volume do Recipiente
                // Quantidade de Passos que o Motor precisa Andar para chegar no Recipiente
                int xDelta = xAlvo - xPega; // Delta X
                int yDelta = yAlvo - yPega; // Delta Y
                
                // Move o Motor até o Recipiente
                MPx.MotorDosagem(xDelta);
                MPy.MotorDosagem(yDelta);
                pulsoPipeta(zAlvo);  // Solta o Líquido da Pipeta para a Recipiente
                
                // Move o Motor até o Pega
                MPx.MotorDosagem(-1 * xDelta);
                MPy.MotorDosagem(-1 * yDelta);
                pulsoPipeta(zPega); // Pega o Líquido do Pega para a Pipeta
                
                volRecip++;
            }   
            recip++;
        }
//        estado = 4; // Vai para o Finalização | IHM
    } // FIM DO ESTADO DE DOSAGEM
    
    while(estado == 4) { // FINALIZAÇÃO
        printf("\rObrigado, Ate Mais Tarde!\n");
    } // FIM DO ESTADO DE FINALIZAÇÃO 
}


// FUNÇÕES:
void etapaEmerg(void) { // Entra no Estado de Emergência | Emergência
    while(Emerg == 0) { // Continua em Emergência até que o Botão mude de Estado
        printf("\remergencia acionada\n");
    }
}

void JS_Posicionamento(int x, int y) { // Move os Motores de Acordo com Inputs do JS
    if(x < 500 - JS.offset){ // Detecta se o JS Está para a Esquerda
        MPx.MoverMotor(1); // Move -x
    }
    if(x > 500 + JS.offset) { // Detecta se o JS Está para a Diretia
        MPx.MoverMotor(0); // Move +x
    }
    
    if(y < 500 - JS.offset){ // Detecta se o JS Está para Baixo
        MPy.MoverMotor(1); // Move -y
    }
    if(y > 500 + JS.offset) { // Detecta se o JS Está para Cima
        MPy.MoverMotor(0); // Move +y
    }
}

void pulsoPipeta(int z) { // Move e Emite Pulso da Pipeta | Dosagem
//    MPz.MotorPorPasso(z, 1);
//    MPz.MotorPorPasso(z, 0);
    
}
