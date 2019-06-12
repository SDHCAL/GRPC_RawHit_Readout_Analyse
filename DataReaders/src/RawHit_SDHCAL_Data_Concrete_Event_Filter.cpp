#include "RawHit_SDHCAL_Data_Concrete_Event_Filter.h"

#include <sstream>
#include <map>
#include <set>

std::string Both_Reject_Filter::name()
{
  std::stringstream ss;
  ss << " Both rejection of [" <<  m_filterOne.name() << "] and [" << m_filterTwo.name() << "]";
  return ss.str();
}

bool Both_Reject_Filter::reject(const RawHit_SDHCAL_Data& d)
{
  return m_filterOne.reject(d) && m_filterTwo.reject(d);
}

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

std::string NoHitInLayers_Filter::name()
{
  std::stringstream ss;
  ss << "Reject events with no hits in the following layers :";
  ss << m_layersToCheck;
  return ss.str();
}

bool NoHitInLayers_Filter::reject(const RawHit_SDHCAL_Data& d)
{
  for (auto hit : d.getHitVector())
    if ( m_layersToCheck.valueInDomain(m_setup.getPlanNumberSafe(hit.dif())) )
      return false;
  return true;
}


std::string ConsecutiveLayers_Filter::name()
{
  std::stringstream ss;
  ss << "Reject events with max number of consecutive fired plans less or equal than ";
  ss << m_minNumberOfPlanToKeep <<".";
  return ss.str();
}

bool ConsecutiveLayers_Filter::reject(const RawHit_SDHCAL_Data& d)
{
  std::set<int> firedPlan;
  for (auto hit : d.getHitVector())
    {
      unsigned int dif=hit.dif();
      if (m_setup.DIFnumberIsBIF(dif)) continue;
      firedPlan.insert(m_setup.getPlanNumber(dif));
    }
  int maxConsecutiveLayers = 0 ;
  int currentCtr = 1 ;
  int previousOk = std::numeric_limits<int>::min() ;
  bool bonusAllowed = false ;
  for ( std::set<int>::iterator it = firedPlan.begin() ; it != firedPlan.end() ; ++it )
    {
      if ( *it == previousOk + 1 )
	{
	  ++currentCtr ;
	  if ( currentCtr > maxConsecutiveLayers )
	    maxConsecutiveLayers = currentCtr ;
	}
      else if ( *it == previousOk + 2 && !bonusAllowed )
	{
	  bonusAllowed = true ;
	  ++currentCtr ;
	  if ( currentCtr > maxConsecutiveLayers )
	    maxConsecutiveLayers = currentCtr ;
	}
      else
	{
	  currentCtr = 1 ;
	  bonusAllowed = false ;
	}
      
      previousOk = *it ;
      if ( maxConsecutiveLayers > m_minNumberOfPlanToKeep )
	 return false;
    }
  return true;
}

