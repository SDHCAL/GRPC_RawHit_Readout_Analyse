#include "TricotDevice.h"

std::pair<unsigned int, unsigned int> TricotDevice::getIJ(unsigned int dif, unsigned int asic, unsigned int channel) const
{
  return std::make_pair(channel,asic);
}

DIFdrivenDevice::dif_asic_channel TricotDevice::getDifAsicChannel(unsigned int Strip, unsigned int Angle) const
{
  return dif_asic_channel(m_DIFnumbers[0],Angle,Strip);
}
