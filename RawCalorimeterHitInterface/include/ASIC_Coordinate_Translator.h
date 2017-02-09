#ifndef ASIC_Coordinate_Translator_HH
#define ASIC_Coordinate_Translator_HH

#include "Mapping.h"

class ASIC_Coordinate_Translator
{
 public:
  //translate stuff for HR2 pad ASU 
  static unsigned int I(unsigned int asic, unsigned int channel)  {return 1+MapILargeHR2[channel]+AsicShiftI[asic];}
  static unsigned int J(unsigned int asic, unsigned int channel)  {return 32-(MapJLargeHR2[channel]+AsicShiftJ[asic]);}
  static unsigned int asic(unsigned int I, unsigned int J) {return MapAsicLargeHR2[(I-1)/8][(J-1)/8];}
  static unsigned int channel(unsigned int I, unsigned int J) {return MapChannelLargeHR2[(I-1)%8][J%8==0 ? 0 : 8-J%8];}

  
  //translate stuff for HR2 strip ASU 
  static unsigned int stripNumberEven(unsigned int channel) {return 2*channel;} //pair
  static unsigned int stripNumberOdd(unsigned int channel)  {return 2*(64-channel)-1;} //impair
  static unsigned int channelstrip(unsigned int stripnumber) {return (stripnumber%2) == 0 ? stripnumber/2 : (127-stripnumber)/2;}
};



#endif
