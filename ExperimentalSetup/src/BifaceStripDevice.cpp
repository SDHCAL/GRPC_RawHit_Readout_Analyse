#include "BifaceStripDevice.h"

#include "ASIC_Coordinate_Translator.h"

std::pair<unsigned int, unsigned int> BifaceStripDevice::getIJ(unsigned int dif, unsigned int asic, unsigned int channel) const
{ 
  unsigned int I;
  if (dif==m_DIFnumbers[0]) I=ASIC_Coordinate_Translator::stripNumberEven(channel);    
  if (dif==m_DIFnumbers[1]) I=ASIC_Coordinate_Translator::stripNumberOdd(channel);
  return make_pair(I,asic);
}

DIFdrivenDevice::dif_asic_channel BifaceStripDevice::getDifAsicChannel(unsigned int Strip, unsigned int Gap) const
{
  return dif_asic_channel(m_DIFnumbers[Strip%2],Gap,ASIC_Coordinate_Translator::channelstrip(Strip));
}
