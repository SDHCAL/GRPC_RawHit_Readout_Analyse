#include "RawHit_SDHCAL_Data_Concrete_Event_Filter.h"

#include <sstream>
#include <map>

std::string RamFull_Filter::name()
{
  std::stringstream ss;
  ss << "RamFull filter : reject if N_hit in one DIF for channels 29 and 31 > " << m_MinNumberOfRamFullHitToReject;
  ss << " and if fraction of ramfull hit > " << m_minRamFullHitFractionToReject;
  return ss.str();
}

bool RamFull_Filter::reject(const RawHit_SDHCAL_Data& d)
{
  std::map<unsigned int,unsigned int> dif_count;
  int sum_count=0;
  for (auto &hit : d.getHitVector())
    {
      if (hit.channel()==29 || hit.channel()==31) {dif_count[hit.dif()]++;++sum_count;}
    }
  if (float(sum_count)/d.getHitVector().size() > m_minRamFullHitFractionToReject)
    for (auto dif : dif_count)
      if (dif.second >  m_MinNumberOfRamFullHitToReject)
	return true;
  return false;
}
