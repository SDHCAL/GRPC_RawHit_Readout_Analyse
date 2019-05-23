#include "TricotDevice.h"
#include "RawCalorimeterHitPointer.h"

std::pair<unsigned int, unsigned int> TricotDevice::getIJ(unsigned int dif, unsigned int asic, unsigned int channel) const
{
  return std::make_pair(channel,asic);
}

DIFdrivenDevice::dif_asic_channel TricotDevice::getDifAsicChannel(unsigned int Strip, unsigned int Angle) const
{
  return dif_asic_channel(m_DIFnumbers[0],Angle,Strip);
}

void TricotDevice::computeHitPosition(const std::vector<RawCalorimeterHitPointer>& raw)
{
  if (! m_hitsPosition.empty())
    m_hitsPosition.clear();
  //not really implemented
  for (auto &hit : raw) m_hitsPosition[map_index(hit.channel(),hit.asic())]=std::make_pair(float(hit.channel()),float(hit.asic()));
}
