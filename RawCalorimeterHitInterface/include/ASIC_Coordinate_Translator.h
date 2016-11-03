#ifndef ASIC_Coordinate_Translator_HH
#define ASIC_Coordinate_Translator_HH

#include "Mapping.h"

class ASIC_Coordinate_Translator
{
 public:
  //translate stuff for HR2 pad ASU 
  static unsigned int I(unsigned int asic, unsigned int channel)  {return 1+MapILargeHR2[channel]+AsicShiftI[asic];}
  static unsigned int J(unsigned int asic, unsigned int channel)  {return 32-(MapJLargeHR2[channel]+AsicShiftJ[asic]);}

  //translate stuff for HR2 strip ASU 
  static unsigned int stripNumberEven(unsigned int channel) {return 2*channel;} //pair
  static unsigned int stripNumberOdd(unsigned int channel)  {return 2*(64-channel)-1;} //impair
};

#endif
