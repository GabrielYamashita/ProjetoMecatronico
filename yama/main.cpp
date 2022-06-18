
// BIBLIOTECAS:
#include "mbed.h" // Importa a Biblioteca do mbed
#include "MotorPasso.h" // Importa Arquivo da Classe do Motor de Passo
#include "Joystick.h" // Importa Aqrquivo da Classe do Joystick
#include "math.h"
//#include "TelasLCD.h" // Importa as Funções das Telas do LCD

// OBJETOS DE CLASSE PARA A MÁQUINA:
MP MPx(PB_2, PB_1, PB_15, PB_14, PA_11, PA_12); // Bobina1, Bobina2, Bobina3, Bobina4, FimDeCursoInicial, FimDeCursoFinal | x
MP MPy(PA_5, PA_6, PA_7, PB_6, PB_13, PC_4); // Bobina1, Bobina2, Bobina3, Bobina4, FimDeCursoInicial, FimDeCursoFinal | y
MP MPz(PB_9, PB_8, PC_9, PC_8, PC_5, PC_6); // Bobina1, Bobina2, Bobina3, Bobina4, FimDeCursoInicial, FimDeCursoFinal | z

Joystick JS(A4, A5, PC_13, PA_8, PB_10); // x, y, botão | joystick


// PORTAS SERIAIS:
Serial pc(USBTX, USBRX); // 
Serial display(D8, D2, 9600); // tx, rx, baud | Display

// GPIO's:
InterruptIn Emerg(PC_12); // Botão de Emergência
DigitalOut Rele(PC_10, 1); // Relé para Acionamento da Pipeta

// VARIÁVEIS:
int xAtual; // Coord x Atual do Motor | x
int yAtual; // Coord x Atual do Motor | y
int zAtual; // Coord x Atual do Motor | z

int X[16];
int Y[16];
int Z[16];
int V[16];

int soltoX = 0;
int soltoY = 0;
bool soltoFlag = false;

int INDEX = 1;
int volume; // volume de trasferencia do frasco atual

// VARIÁVEIS DO LCD
char rx_line;

float machine_xaxis_size = 25802.0;
float machine_yaxis_size = 9364.0;
float machine_zaxis_size = 4120.0;
int coleta_idx = 1; // indice do frasco atual (sendo selecionado)
int tempo_dosagem = 2; // tempo real do ciclo em min

float x_percent;
float y_percent;
float z_percent;

int x_relative;
int y_relative;

float x_cartesian;
float y_cartesian;
float z_cartesian;

// INICIALIZAÇÃO DE FUNÇÕES (MAIN):
void etapaEmerg(void); // Inicialização da Função do Estado de Emergência | Emergência
void JS_Posicionamento(int x, int y);
void pulsoPipeta(int z); // Inicialização da Função para Mover e Emitir Pulso da Pipeta | Dosagem

// INICIALIZAÇÃO DE FUNÇÕES (LCD):
void Rx_interrupt();
void SendText(char* page); //manda strings pela serial para o display
void DisplayCircle(int PassosX, int PassosY);
void DisplayProgressBar(int time);
void DisplayXYZ_Pega(int x, int y, int z);
void DisplayXYZ_Solta(int x, int y, int z, int V, int Recip);

// ESTADOS DE MÁQUINA:
int estado = 6;


// LOOP PRINCIPAL:
int main(){ // toda vez q chegar(Rx) info pela serial, execura a funcao interupt
    display.attach(&Rx_interrupt, Serial::RxIrq);
    //Emerg.fall(&etapaEmerg); // Aciona o Estado de Emergência
//    
//    while(Emerg == 0) {} // Continua em Emergência até que o Botão mude de Estado
//    SendText("page 0");
//    estado = 0;
    
    while(1) {
        while(estado == 2) { // REFERÊNCIAMENTO
            SendText("t1.txt=\"REFERENCIANDO EIXO X ...\""); // Mudando Texto do Display | x
            MPx.MotorReferenciamento(0); // Realiza o Referenciamento pro 0 do eixo X | x
            
            SendText("t1.txt=\"REFERENCIANDO EIXO Y ...\""); // Mudando Texto do Display | x
            MPy.espera = 5.5;
            MPy.MotorReferenciamento(0); // Realiza o Referenciamento pro 0 do eixo Y | y
            
            SendText("t1.txt=\"REFERENCIANDO EIXO Z ...\""); // Mudando Texto do Display | x
            MPz.espera = 4;
            MPz.MotorReferenciamento(0); // Realiza o Referenciamento pro 0 do eixo Z | z
            
//            SendText("t1.txt=\"CONTANDO OS PASSOS ...\""); // Mudando Texto do Display | x
//            int qntPassosX = MPx.MotorTamanhoFuso(1);
//            int qntPassosY = MPy.MotorTamanhoFuso(1);
//            int qntPassosZ = MPz.MotorTamanhoFuso(1);
//            printf("\rX = %i | Y = %i |  Z = %i\n", qntPassosX, qntPassosY, qntPassosZ);
            
            // Mudando de Tela no Display --> INTRO POSICIONAMENTO (PEGA)
            SendText("page 3");
            estado = 3;
        } // FIM DO ESTADO DE REFERÊNCIAMENTO   
        
        
        while(estado == 4) { // POSICIONAMENTO (PEGA)
            int JSx = JS.GetXValue(); // Pega a Direção x do JS | JS
            int JSy = JS.GetYValue(); // Pega a Direção y do JS | JS
            JS_Posicionamento(JSx, JSy); // Move os Motores de Acordo com o JS
//            pc.printf("\rX = %i | Y = %i - (%i, %i)\n", MPx.QntPassos, MPy.QntPassos, JSx, JSy);
            
            if(soltoX > 10 & soltoY > 10) {
                if(soltoFlag == false) {
                    soltoFlag = true;
                    // Ponto do Pega:
                    pc.printf("SOLTO");
//                    SendText("page 4");

//                    DisplayCircle(MPy.QntPassos, MPx.QntPassos); // bota circulo vermelho da pos do pega
//                    DisplayXYZ_Pega(MPy.QntPassos, MPx.QntPassos, MPz.QntPassos); // muda caixa de texto do x y z 
                }
            }
        } // FIM DO ESTADO DE POSICIONAMENTO (PEGA)
        
        
        while(estado == 6) { // POSICIONAMENTO (SOLTAS)
            int JSx = JS.GetXValue(); // Pega a Direção x do JS | JS
            int JSy = JS.GetYValue(); // Pega a Direção y do JS | JS
            JS_Posicionamento(JSx, JSy); // Move os Motores de Acordo com o JS
//            pc.printf("\rX = %i | Y = %i - (%i, %i)\n", MPx.QntPassos, MPy.QntPassos, JSx, JSy);
            
            if(soltoX > 15 & soltoY > 15) {
                if(soltoFlag == false) {
                    soltoFlag = true;
                    // Ponto do Coleta:
                    SendText("page 6");
                    DisplayCircle(MPy.QntPassos, MPx.QntPassos); // bota circulo vermelho da pos do pega
                    DisplayXYZ_Solta(MPy.QntPassos, MPx.QntPassos, MPz.QntPassos, volume, coleta_idx); // muda caixa de texto do x y z v recip
                }
            }
        } // FIM DO ESTADO DE POSICIONAMENTO (SOLTAS)
        
        
        while(estado == 8) { // DOSAGEM
            // Define as Coordenadas do Pega
            int xPega = X[0];
            int yPega = Y[0];
            int zPega = Z[0];
    
            // Move o Motor para as Coordenadas do Pega
            MPx.MotorDosagem(xPega - X[INDEX]);
            MPy.MotorDosagem(yPega - Y[INDEX]);
            pulsoPipeta(zPega); // Pega o Líquido do Pega para a Pipeta
            
            int recip = 1; // Começa no Índice 1 da Lista, dos Recipientes/Soltas
            while(recip < INDEX) { // Roda com o Tamanho da lista dos Recipientes
                
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
            SendText("page 9");
            estado = 9;
        } // FIM DO ESTADO DE DOSAGEM
    }
}


// FUNÇÕES (MAIN):
void etapaEmerg(void) { // Entra no Estado de Emergência | Emergência
    SendText("page 10");
    estado = 10;
    NVIC_SystemReset();
}

void JS_Posicionamento(int x, int y) { // Move os Motores de Acordo com Inputs do JS
    // Movimento do Eixo X
    if(x < 500 - JS.offset){ // Detecta se o JS Está para a Esquerda
        soltoX = 0;
        soltoFlag = false;
        
        MPx.MoverMotor(0); // Move -x
        MPx.QntPassos--;
        if(MPx.QntPassos < 0) {MPx.QntPassos = 0;}

    } else if(x > 500 + JS.offset) { // Detecta se o JS Está para a Diretia
        soltoX = 0;
        soltoFlag = false;
        
        MPx.MoverMotor(1); // Move +x
        MPx.QntPassos++;

    } else {
        MPx.MotorPasso = 0;
        soltoX++;
    }
    
    
    // Movimento do Eixo Y
    if(y > 500 + JS.offset) { // Detecta se o JS Está para Baixo
        soltoY = 0;
        soltoFlag = false;
        
        MPy.MoverMotor(1); // Move +y
        MPy.QntPassos++;

    } else if(y < 500 - JS.offset){ // Detecta se o JS Está para Cima
        soltoY = 0;
        soltoFlag = false;
        
        MPy.MoverMotor(0); // Move -y
        MPy.QntPassos--;
        if(MPy.QntPassos < 0) {MPy.QntPassos = 0;}
        
    } else {
        MPy.MotorPasso = 0;
        soltoY++;
    }
    
    if(JS.Button() == 1) {
        pc.printf("Botao Pressionado");
    }
}

void pulsoPipeta(int z) { // Move e Emite Pulso da Pipeta | Dosagem
    MPz.MotorDosagem(z);
    Rele = 1;
    wait_ms(0.05);
    Rele = !Rele;
    wait_ms(1700);
    MPz.MotorDosagem(z); 
}


// FUNÇÕES (LCD)
void Rx_interrupt() { // Recebe os Outputs dos Botões do display 
    while(display.readable()){
        rx_line = display.getc(); // Recebe o char do Buffer Usado pelo Dispositivo "display"
        pc.putc(rx_line); // Manda o char pela Serial PC
        
        switch(rx_line) {
            case 'z': // MENU --> INTRO REFERÊNRENCIAMENTO
                SendText("page 1");
                estado = 1;
                rx_line = 0x00;
                break;
    
            case 'a': // INTRO REFERÊNRENCIAMENTO --> REFERÊNRENCIAMENTO
                SendText("page 2");
                estado = 2;
                rx_line = 0x00;
                break;
                
            case 'b': // INTRO POSICIONAMENTO (PEGA) --> POSICIONAMENTO (PEGA)
                SendText("page 4");
                estado = 4;
                break;
                
            case 'c': // POSICIONAMENTO (PEGA) --> INTRO POSICIONAMENTO (SOLTA)
                // Salva a Posição do Pega
                X[INDEX] = MPx.QntPassos;
                Y[INDEX] = MPy.QntPassos;
                Z[INDEX] = MPz.QntPassos;
                V[INDEX] = 0;
                INDEX++;
            
                SendText("page 5");
                estado = 5;
                break;
                
            case 'd': // INTRO INTRO POSICIONAMENTO (PEGA) --> POSICIONAMENTO (PEGA)
                SendText("page 6");
                estado = 6;
                break;
                
            case 'e': // Selecionar mais uma posicao de tranferência
                // Salva os Pontos do Solta
                X[INDEX] = MPy.QntPassos;
                Y[INDEX] = MPx.QntPassos;
                Z[INDEX] = MPz.QntPassos;
                V[INDEX] = volume;
                INDEX++;
                coleta_idx++;
            
                SendText("page 6"); // recarrega a pagina -> zera os valores
                estado = 6;
                break;
                
            case 'f': // Finalizar seleção das posições de tranferência
                // Salva o Último Ponto do Solta
                X[INDEX] = MPy.QntPassos;
                Y[INDEX] = MPx.QntPassos;
                Z[INDEX] = MPz.QntPassos;
                V[INDEX] = volume;
                INDEX++;
            
                SendText("page 7"); // pagina pra comecar o processo de pipetagem
                estado = 7;
                break;
                
            case 'g': // Aumenta 1ml no volume de transferência
                volume++;
                break;
                
            case 'h': // Diminui 1ml no volume de transferência
                volume--;
                break;
                
            case 'i': // INTRO DOSAGEM --> DOSAGEM
                SendText("page 8"); // pagina pra comecar o processo de pipetagem
                estado = 8;
                break;
            
            case 'j': // FINALIZAÇÃO --> INTRO POSICIONAMENTO (PEGA)
                SendText("page 3"); // pagina pra comecar o processo de pipetagem
                estado = 3;
                break;
                
            case 'k': // FINALIZAÇÃO --> INTRO DOSAGEM
                SendText("page 7"); // pagina pra comecar o processo de pipetagem
                estado = 7;
                break;
                
            case 'l':
                SendText("page 0");
                estado = 0;
                break;
                  
            default: rx_line=0x00;
        }
    return;
    }
}

void SendText(char* text){ // Manda Informação para o LCD
    display.printf(text);
    display.putc(0xFF); // Carácter Especial
    display.putc(0xFF); // Carácter Especial
    display.putc(0xFF); // Carácter Especial
    wait(0.05);
}

void DisplayCircle(int PassosX, int PassosY){ // Desenha um Círculo no Gráfico da Rotinas dos Poscionamentos
    char txt[100];
    
    x_percent = PassosX/machine_yaxis_size; // Calcula a Porcentagem | x
    y_percent = PassosY/machine_xaxis_size; // Calcula a Porcentagem | y
    
    x_relative = (int)(250 + x_percent * 250); // Calcula a Coordenada Relativa do Cartesiano | x
    y_relative = (int)(130 + (1 - y_percent) * 243); // Calcula a Coordenada Relativa do Cartesiano | y

    // Manda a Coordenada do Círculo para o LCD
    sprintf(txt,"cirs %d,%d,5,RED", x_relative, y_relative);
    pc.printf(txt);
    SendText(txt);   
}

void DisplayXYZ_Pega(int PassosX, int PassosY, int PassosZ){  // Manda as Informações pro LCD da Rotina de Posicionamento do Pega
    char txt[100];
    
    x_percent = PassosY/machine_xaxis_size; // Calcula a Porcentagem | x
    y_percent = PassosX/machine_yaxis_size; // Calcula a Porcentagem | y
    z_percent = PassosZ/machine_zaxis_size; // Calcula a Porcentagem | z
    pc.printf("\rX PER = %f | Y PER = %f | Z PER = %f\n", x_percent, y_percent, z_percent);
    
    x_cartesian = x_percent * 10; // Calcula a Dimensão no Cartesiano do LCD | x
    y_cartesian = y_percent * 10; // Calcula a Dimensão no Cartesiano do LCD | y
    z_cartesian = z_percent * 10; // Calcula a Dimensão no Cartesiano do LCD | z
    pc.printf("\rX CAR = %f | Y CAR = %f | Z CAR = %f\n", x_cartesian, y_cartesian, z_cartesian);
    
    // Manda a Coordenada Cartesiana no LCD | x
    sprintf(txt,"n0.val=%2f", x_cartesian);
    pc.printf(txt);
    SendText(txt);
    
    // Manda a Coordenada Cartesiana no LCD | y
    sprintf(txt,"n1.val=%2f", y_cartesian); 
    pc.printf(txt);
    SendText(txt);
    
    // Manda a Coordenada Cartesiana no LCD | z
    sprintf(txt,"n2.val=%2f", z_cartesian); 
    pc.printf(txt);
    SendText(txt);
}

void DisplayXYZ_Solta(int x, int y, int z, int V, int Recip){ // Manda as Informações pro LCD da Rotina de Posicionamento dos Soltas
    char txt[100];
    
    x_percent = x/machine_xaxis_size; // Calcula a Porcentagem | x
    y_percent = y/machine_yaxis_size; // Calcula a Porcentagem | y
    y_percent = y/machine_yaxis_size; // Calcula a Porcentagem | z
    
    x_cartesian = x_percent * 10; // Calcula a Dimensão no Cartesiano do LCD | x
    y_cartesian = y_percent * 10; // Calcula a Dimensão no Cartesiano do LCD | y
    z_cartesian = z_percent * 10; // Calcula a Dimensão no Cartesiano do LCD | z
    
    // Manda a Coordenada Cartesiana no LCD | x
    sprintf(txt,"n0.val=%f", x_cartesian);
    pc.printf(txt);
    SendText(txt);
    
    // Manda a Coordenada Cartesiana no LCD | y
    sprintf(txt,"n1.val=%f", y_cartesian); 
    pc.printf(txt);
    SendText(txt);
    
    // Manda a Coordenada Cartesiana no LCD | z
    sprintf(txt,"n2.val=%f", z_cartesian); 
    pc.printf(txt);
    SendText(txt);
    
    // Manda o Recipiente Atual de Transeferência pro LCD
    sprintf(txt,"n3.val=%d", Recip); 
    pc.printf(txt);
    SendText(txt);
    
    // Manda a Quantidade de Volume da Transferência pro LCD
    sprintf(txt,"n4.val=%d", V); 
    pc.printf(txt);
    SendText(txt);
}
