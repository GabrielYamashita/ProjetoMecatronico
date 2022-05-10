#include "MotorPasso.h"

MP::MP(PinName a, PinName b, PinName c, PinName d) : MotorPasso(a, b, c, d) {
   this->espera = 2;
}

void MP::MoverMotor(int Dir) {
   if (Dir == 0) {
      for(int i = 3; i > -1; i--) {
         MotorPasso = 1 << i;
         wait_ms(espera);
      }
   } else {
      for(int i = 0; i < 4; i++) {
         MotorPasso = 1 << i;
         wait_ms(espera);
      }
   }
}