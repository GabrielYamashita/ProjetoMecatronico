#include "mbed.h"
#include "MotorPasso.h"

MP MPx(D3, D4, D5, D6);

int main(){
   while(1) {
      MPx.MoverMotor(1);
   }
}