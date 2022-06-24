
// BIBLIOTECAS E IMPORTS:
#include "mbed.h" // Importa a Biblioteca do mbed
#include "MotorPasso.h" // Importa Arquivo da Classe do Motor de Passo
#include "Joystick.h" // Importa Aqrquivo da Classe do Joystick
//#include "TelasLCD.h" // Importa as Funções das Telas do LCD

// OBJETOS DE CLASSE PARA A MÁQUINA:
MP MPx(PB_2, PB_1, PB_15, PB_14, PA_11, PA_12); // Bobina1, Bobina2, Bobina3, Bobina4, FimDeCursoInicial, FimDeCursoFinal | x
MP MPy(PA_5, PA_6, PA_7, PB_6, PB_13, PC_4); // Bobina1, Bobina2, Bobina3, Bobina4, FimDeCursoInicial, FimDeCursoFinal | y
MP MPz(PB_9, PB_8, PC_9, PC_8, PC_5, PC_6); // Bobina1, Bobina2, Bobina3, Bobina4, FimDeCursoInicial, FimDeCursoFinal | z

Joystick JS(A4, A5, PC_13); // x, y, botão | joystick

// PORTAS SERIAIS:
Serial pc(USBTX, USBRX); // 
Serial display(D8, D2, 9600); // tx, rx, baud | Display

// GPIO's:
InterruptIn Emerg(PC_12); // Botão de Emergência
DigitalIn zMenos(PA_8); // Botão do Eixo Z+
DigitalIn zMais(PB_10); // Botão do Eixo Z-
DigitalOut Rele(PC_10, 1); // Relé para Acionamento da Pipeta

// VARIÁVEIS:
//int X[16]; // Lista de Coordenadas dos Recipientes | x
//int Y[16]; // Lista de Coordenadas dos Recipientes | y
//int Z[16]; // Lista de Coordenadas dos Recipientes | z
//int V[16]; // Lista de Volumes dos Recipientes | V

int X[16] = {3923, 3232, 2424, 4242, 2442, 2225};
int Y[16] = {2424, 6332, 8984, 9892, 2424, 9822};
int Z[16] = {4902, 2482, 2984, 2498, 9829, 9998};
int V[16] = {1, 5, 3, 4, 6, 1};

int soltoX = 0; // Checa se o Eixo do JS está Solto | x
int soltoY = 0; // Checa se o Eixo do JS está Solto | y
bool soltoFlag = false; // Flag para Checar se ja foi Solto os Eixos do JS

int INDEX = 0; // Index da Lista de x, y, z e V
int volume; // Volume de Trasferência do Frasco Atual

// VARIÁVEIS DO LCD:
char rx_line;

float machine_xaxis_size = 25802.0; // Tamanho Total do Eixo em Passos | x
float machine_yaxis_size = 9364.0; // Tamanho Total do Eixo em Passos | y
float machine_zaxis_size = 4120.0; // Tamanho Total do Eixo em Passos | z
int coleta_idx = 1; // Índice do Frasco Atual (Sendo Selecionado) dos Soltas

float x_percent; // Coord do Eixo em Porcentagem | x
float y_percent; // Coord do Eixo em Porcentagem | y
float z_percent; // Coord do Eixo em Porcentagem | z

int x_relative; // Coord do Eixo Relativo em Pixels | x
int y_relative; // Coord do Eixo Relativo em Pixels | y

int x_cartesian; // Coord do Eixo Cartesiano [0, 100] | x
int y_cartesian; // Coord do Eixo Cartesiano [0, 100] | y
int z_cartesian; // Coord do Eixo Cartesiano [0, 100] | z

// INICIALIZAÇÃO DE FUNÇÕES (MAIN):
void etapaEmerg(void); // Inicialização da Função do Estado de Emergência | Emergência
void JS_Posicionamento(int x, int y, int z); // Inicialização da Função do Posicionamento do JoyStick | Posicionamento
void pulsoPipeta(int z); // Inicialização da Função para Mover e Emitir Pulso da Pipeta | Dosagem

// INICIALIZAÇÃO DE FUNÇÕES (LCD):
void Rx_interrupt();
void SendText(char* page); // Inicialização da Função para Mandar String pelo Serial do Display
void DisplayCircle(int PassosX, int PassosY); // Inicialização da Função para Desenhar um Círculo no Display
void DisplayXYZ_Pega(int x, int y, int z); // Inicialização da Função para Mandar Infos para o Display | Posicionamento (Pega)
void DisplayXYZ_Solta(int x, int y, int z, int V, int Recip); // Inicialização da Função para Mandar Infos para o Display | Posicionamento (Soltas)

// ESTADOS DE MÁQUINA:
int estado;


// LOOP PRINCIPAL:
int main() { // LOOP PRINCIPAL (MAIN)
    display.attach(&Rx_interrupt, Serial::RxIrq); // Toda Vez que Chegar Info pela serial (Rx), Executa a Funço Interrupt (Rx_interrupt)
//    Emerg.fall(&etapaEmerg); // Aciona o Estado de Emergência, quando o Botão é Pressionado
//    
//    while(Emerg == 0) {} // Continua em Emergência até que o Botão seja Solto
//    SendText("page 0"); // Encaminha Para a Página do Menu Inicial
    estado = 8; // Muda Estado para 0
    
    while(1) { // LOOP DE ESTADOS
        while(estado == 2) { // REFERÊNCIAMENTO
            SendText("t1.txt=\"REFERENCIANDO EIXO Z ...\""); // Mudando Texto do Display | z
            MPz.espera = 4; // Altera a Espera do Motor Z
            MPz.MotorReferenciamento(0); // Realiza o Referenciamento pro 0 | z
        
            SendText("t1.txt=\"REFERENCIANDO EIXO X ...\""); // Mudando Texto do Display | x
            MPx.MotorReferenciamento(0); // Realiza o Referenciamento pro 0 | x
            
            SendText("t1.txt=\"REFERENCIANDO EIXO Y ...\""); // Mudando Texto do Display | y
            MPy.espera = 5.5; // Altera a Espera do Motor Y
            MPy.MotorReferenciamento(0); // Realiza o Referenciamento pro 0 | y
            
            // Mudando de Tela no Display --> INTRO POSICIONAMENTO (PEGA)
            SendText("page 3");
            estado = 3;
        } // FIM DO ESTADO DE REFERÊNCIAMENTO   
        
        
        while(estado == 4) { // POSICIONAMENTO (PEGA)
            int JSx = JS.GetXValue(); // Pega a Direção x | JS
            int JSy = JS.GetYValue(); // Pega a Direção y | JS
            int JSz = 0;
            
            if(zMais == 1) { // Botão do Z+ Pressionado
                JSz = 1; // +Z
            } else if(zMenos == 1){ // Botão do Z- Pressionado
                JSz = -1;// -Z
            }
            
            JS_Posicionamento(JSx, JSy, JSz); // Move os Motores de Acordo com o JS
            
            if(soltoX > 75 & soltoY > 75) { // Checa se o JS Está Soltoo por mais de 75 Loops
                if(soltoFlag == false) { // Checa se ja Foi Acionado Uma Vez pela Flag
                    soltoFlag = true; // Define como a Flag Acionada
                    SendText("page 4"); // Atualiza a Página 
                    DisplayCircle(MPy.QntPassos, MPx.QntPassos); // Desenha o Círculo Vermelho
                    DisplayXYZ_Pega(MPy.QntPassos, MPx.QntPassos, MPz.QntPassos); // Muda a Caixa de Texto do x, y e z 
                    
                } else if(zMenos == 0 & zMais == 0) { // Checa se os Dois Botões de Z Estão Soltos
                    DisplayXYZ_Pega(MPy.QntPassos, MPx.QntPassos, MPz.QntPassos); // Muda a Caixa de Texto do x, y e z
                }
            }
        } // FIM DO ESTADO DE POSICIONAMENTO (PEGA)
        
        
        while(estado == 6) { // POSICIONAMENTO (SOLTAS)
            int JSx = JS.GetXValue(); // Pega a Direção x do JS | JS
            int JSy = JS.GetYValue(); // Pega a Direção y do JS | JS
            int JSz = 0;
            
            if(zMais == 1) { // Botão do Z+ Pressionado
                JSz = 1; // +Z
            } else if(zMenos == 1){ // Botão do Z- Pressionado
                JSz = -1;// -Z
            }
            
            JS_Posicionamento(JSx, JSy, JSz); // Move os Motores de Acordo com o JS
            
            if(soltoX > 75 & soltoY > 75) { // Checa se o JS Está Soltoo por mais de 75 Loops
                if(soltoFlag == false) { // Checa se ja Foi Acionado Uma Vez pela Flag
                    soltoFlag = true; // Define como a Flag Acionada
                    SendText("page 6"); // Atualiza a Página 
                    DisplayCircle(MPy.QntPassos, MPx.QntPassos); // Desenha o Círculo Vermelho
                    DisplayXYZ_Solta(MPx.QntPassos, MPy.QntPassos, MPz.QntPassos, volume, coleta_idx); // Muda a Caixa de Texto do x, y e z 
                }
                else if(zMenos == 0 & zMais == 0) { // Checa se os Dois Botões de Z Estão Soltos
                    DisplayXYZ_Solta(MPy.QntPassos, MPx.QntPassos, MPz.QntPassos, volume, coleta_idx); // Muda a Caixa de Texto do x, y e z 
                }
            }
            MPz.MotorReferenciamento(0);
        } // FIM DO ESTADO DE POSICIONAMENTO (SOLTAS)
        
        
        while(estado == 8) { // DOSAGEM
            //// Define as Coordenadas do Pega
            int xPega = X[0];
            int yPega = Y[0];
            int zPega = Z[0];
    
            // Move o Motor para as Coordenadas do Pega
            MPx.MotorDosagem(xPega - X[INDEX]);
            MPy.MotorDosagem(yPega - Y[INDEX]);
            pulsoPipeta(zPega); // Pega o Líquido do Pega para a Pipeta
            
            int recip = 1; // Começa no Índice 1 da Lista, dos Recipientes/Soltas
            while(recip < INDEX) { // Roda com o Tamanho da Lista dos Recipientes
                
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
    } // FIM DO LOOP DE ESTADOS
} // FIM DO LOOP PRINCIPAL (MAIN)


// FUNÇÕES (MAIN):
void etapaEmerg(void) { // Entra no Estado de Emergência | Emergência
    SendText("page 10"); // Entra na Página de Emergência
    estado = 10; // Estado de Emergência
    NVIC_SystemReset(); // Reseta o Código para o Main Novamente
}

void JS_Posicionamento(int x, int y, int z) { // Move os Motores de Acordo com Inputs do JS
    bool Flag = false;

    // Movimento do Eixo X        
    if(x > 500 + JS.offset & !Flag & MPx.FCf == 1) { // Detecta se o JS Está para Cima
        soltoX = 0; 
        soltoFlag = false; // Reseta a Flag do Eixo Solto
        Flag = true;
        
        MPx.MoverMotor(1); // Move +x
        MPx.QntPassos++;

    } else if(x < 500 - JS.offset & !Flag & MPx.FCi == 1){ // Detecta se o JS Está para Baixo
        soltoX = 0;
        soltoFlag = false;
        Flag = true;
        
        MPx.MoverMotor(0); // Move -x
        MPx.QntPassos--;
        if(MPx.QntPassos < 0) {MPx.QntPassos = 0;}

    } else {
        MPx.MotorPasso = 0;
        soltoX++;
    }
    
    
    // Movimento do Eixo Y
    if(y > 500 + JS.offset & !Flag & MPy.FCf == 1) { // Detecta se o JS Está para Direita
        soltoY = 0;
        soltoFlag = false;
        Flag = true;
        
        MPy.MoverMotor(1); // Move +y
        MPy.QntPassos++;

    } else if(y < 500 - JS.offset & !Flag & MPy.FCi == 1){ // Detecta se o JS Está para Esquerda
        soltoY = 0;
        soltoFlag = false;
        Flag = true;
        
        MPy.MoverMotor(0); // Move -y
        MPy.QntPassos--;
        if(MPy.QntPassos < 0) {MPy.QntPassos = 0;}
        
    } else {
        MPy.MotorPasso = 0;
        soltoY++;
//        pc.printf("\rSOLTO Y: %i\n", soltoY);
    }
    
    // Movimento do Eixo Z
    if(z == 1 & !Flag & MPz.FCf == 1){ // Detecta se o Botão do Eixo Z+ está Apertado
        MPz.MoverMotor(1);
        MPz.QntPassos++;
        
    } else if(z == -1 & !Flag & MPz.FCi == 1){ // Detecta se o Botão do Eixo Z- está Apertado
        MPz.MoverMotor(0);
        MPz.QntPassos--;
        if(MPz.QntPassos < 0) {MPz.QntPassos = 0;}
    }

    // Detecta se o Botão Está Pressionado
    if(JS.Button() == 1) {
        pc.printf("\rBotao Pressionado\n");
    }
}

void pulsoPipeta(int z) { // Move e Emite Pulso da Pipeta | Dosagem
    MPz.espera = 3; // Define Velocidade do Motor Z
    MPz.MotorDosagem(z); // Desce o Eixo Z
    Rele = 0; // Ativa o Rele
    wait_ms(2000); // Espera pelo Pulso do Rele
    Rele = 1; // Desativa o Rele
    wait_ms(1700); // Espera o Líquido Puxar
    MPz.MotorDosagem(-z); // Sobe o Eixo Z
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
            
            case 'm':
                SendText("page 12");
                estado = 12;
                
            case 'n':
                SendText("page 11");
                estado = 11;
                  
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
    
    if(x_percent > 0.005){
        x_cartesian = (int)(x_percent * 100 + 2); // Calcula a Dimensão no Cartesiano do LCD | x
    } else {
        x_cartesian = 0;
    }
    
    if(y_percent > 0.005){
        y_cartesian = (int)(y_percent * 100 + 2); // Calcula a Dimensão no Cartesiano do LCD | y
    } else {
        y_cartesian = 0;
    }
    
    if(z_percent > 0.005){
        z_cartesian = (int)(z_percent * 100 + 2); // Calcula a Dimensão no Cartesiano do LCD | z
    } else {
        z_cartesian = 0;
    }
    
    // Manda a Coordenada Cartesiana no LCD | x
    sprintf(txt,"n0.val=%d", y_cartesian);
    pc.printf(txt);
    SendText(txt);
    
    // Manda a Coordenada Cartesiana no LCD | y
    sprintf(txt,"n1.val=%d", x_cartesian); 
    pc.printf(txt);
    SendText(txt);
    
    // Manda a Coordenada Cartesiana no LCD | z
    sprintf(txt,"n2.val=%d", z_cartesian); 
    pc.printf(txt);
    SendText(txt);
}

void DisplayXYZ_Solta(int PassosX, int PassosY, int PassosZ, int V, int Recip){ // Manda as Informações pro LCD da Rotina de Posicionamento dos Soltas
    char txt[100];
    
    x_percent = PassosY/machine_xaxis_size; // Calcula a Porcentagem | x
    y_percent = PassosX/machine_yaxis_size; // Calcula a Porcentagem | y
    z_percent = PassosZ/machine_zaxis_size; // Calcula a Porcentagem | z
//    pc.printf("\rX PER = %f | Y PER = %f | Z PER = %f\n", x_percent, y_percent, z_percent);
    
    if(x_percent > 0.005){
        x_cartesian = (int)(x_percent * 100 + 2); // Calcula a Dimensão no Cartesiano do LCD | x
    } else {
        x_cartesian = 0;
    }
    
    if(y_percent > 0.005){
        y_cartesian = (int)(y_percent * 100 + 2); // Calcula a Dimensão no Cartesiano do LCD | y
    } else {
        y_cartesian = 0;
    }
    
    if(z_percent > 0.005){
        z_cartesian = (int)(z_percent * 100 + 2); // Calcula a Dimensão no Cartesiano do LCD | z
    } else {
        z_cartesian = 0;
    }
    
//    pc.printf("\rX CAR = %i | Y CAR = %i | Z CAR = %i\n", x_cartesian, y_cartesian, z_cartesian);
    
    // Manda a Coordenada Cartesiana no LCD | x
    sprintf(txt,"n0.val=%i", x_cartesian);
    pc.printf(txt);
    SendText(txt);
    
    // Manda a Coordenada Cartesiana no LCD | y
    sprintf(txt,"n1.val=%i", y_cartesian); 
    pc.printf(txt);
    SendText(txt);
    
    // Manda a Coordenada Cartesiana no LCD | z
    sprintf(txt,"n2.val=%i", z_cartesian); 
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
