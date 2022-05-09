/***********************************************************************
* 09-05-2022
* 2x Motor-de-passo + Joystick (X-Y)
************************************************************************/
#include "mbed.h"

#define velo 0.0025 // valor do periodo entre passos, para velocidade

// LSB MSB
BusOut MPy(D3, D4, D5, D6);
BusOut MPx(D8, D9, D10, D11);
DigitalOut Led(LED1);

DigitalIn botao(D2); // botão-do-usuário na placa NUCLEO-F103RB

AnalogIn xAxis(A0);
AnalogIn yAxis(A1);


// maximo de passos dos eixos -> calculado na rotina de referenciamento (1 volta ~ 2040 passos)
int xsize = 500; //exemplo pra teste (~ 1/4 de volta)
int ysize = 1000; //exemplo pra teste (~ 1/2 volta)

// passo atual
int xatual = 0;
int yatual = 0; 

// posicao de coleta OBS: apenas uma -> 1 x, 1 y, 1 z
int posicao_coleta[3];


//------------------- funcoes pra mover motor do eixo x ------------------------

void MoveX(int x)
{
//    while(xatual < xsize)
//        {
        if (x > 550)
            {
            // Deslocamento LSB > MSB
            for (int i = 0; i < 4; i++)
                {
                MPx = 1 << i;
                
                xatual++;
                wait(velo);
                }
            }
            
         if (x < 450)
            {
            // Deslocamento LSB > MSB
            for (int i = 3; i > -1; i--)
                {
                MPx = 1 << i;
                xatual--;
                wait(velo);
                }
            }
//        }
}


// funcao pra mover motor do eixo y
void MoveY(int y)
{
    if (y > 550)
        {
        // Deslocamento LSB > MSB
        for (int i = 0; i < 4; i++)
            {
            MPy = 1 << i;
            wait(velo);
            }
        }

    if (y < 450)
        {
        // Deslocamento LSB > MSB
        for (int i = 3; i > -1; i--)
            {
            MPy = 1 << i;
            wait(velo);
            }

        }
}

//---------------------------- rotina de jogging -------------------------------
void Jogging()
{
    int x,y;
  
    while(1)
    {
        x = xAxis.read() * 1000; // float (0->1) to int (0-1000)
        y = yAxis.read() * 1000;
      
        MoveX(x);
        MoveY(y);
    }
}

//--------------------------- funcao que guarda posicoes --------------------------

void GetPosition()
{
    bool estado = 0;
    Led = 0;
    
    // salva o valor 
    while(1)
    {
        if (botao ==0)
        {
            estado = !estado; // inverte o estado
            //salva os valores 
            posicao_coleta[0] = xatual;
            posicao_coleta[1] = yatual;
            
        }
    }
    
    
    // mostra 2 opcoes no display: selecionar de novo / selecionar o proximo
}    
