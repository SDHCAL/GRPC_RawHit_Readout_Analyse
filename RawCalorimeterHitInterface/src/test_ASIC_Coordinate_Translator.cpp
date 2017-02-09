// test program

#include <assert.h>
#include <iostream>

#include "ASIC_Coordinate_Translator.h"

int main()
{
  for (unsigned int asic=1; asic<=48; ++asic)
    for (unsigned int channel=0; channel<64; ++channel)
      {
	unsigned int I=ASIC_Coordinate_Translator::I(asic,channel);
	unsigned int J=ASIC_Coordinate_Translator::J(asic,channel);
	unsigned int rec_channel=ASIC_Coordinate_Translator::channel(I,J);
	unsigned int rec_asic=ASIC_Coordinate_Translator::asic(I,J);
	std::cout << asic << "," << channel << "  <-->  " << I << "," << J << " <--> " << rec_asic << "," << rec_channel << std::endl;
	assert(channel==rec_channel);
	assert(asic==rec_asic);	       
      }
  for (unsigned int channel=0; channel<64; ++channel)
    {
      std::cout << "strip channel " << channel << std::endl;
      assert(channel==ASIC_Coordinate_Translator::channelstrip(ASIC_Coordinate_Translator::stripNumberEven(channel)));
      assert(channel==ASIC_Coordinate_Translator::channelstrip(ASIC_Coordinate_Translator::stripNumberOdd(channel)));
    }
  return 0;
}
