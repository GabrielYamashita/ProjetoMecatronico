#ifndef MOTORPASSO_H
#define MOTORPASSO_H
#include "mbed.h"

class MP {
   public:
      MP(PinName a, PinName b, PinName c, PinName d); // 
        
      void MoverMotor(int Dir);

      BusOut MotorPasso;
      int espera;
};

#endif