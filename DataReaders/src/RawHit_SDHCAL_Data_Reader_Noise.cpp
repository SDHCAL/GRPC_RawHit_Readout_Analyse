#include "RawHit_SDHCAL_Data_Reader_Noise.h"

#include "RawHit_SDHCAL_Data.h"
#include "RawCalorimeterHitPointerTools.h"
#include <set>

const unsigned int RawHit_SDHCAL_Data_Reader_Noise::NO_CUTVALUE=10000000;

RawHit_SDHCAL_Data_Reader_Noise::RawHit_SDHCAL_Data_Reader_Noise(ExperimentalSetup& setup, unsigned int noiseWindowLength) 
  : RawHit_SDHCAL_Data_Reader_FromTimeAnalysis(UI_intervalle(0,m_noiseWindowLength-1)),
    m_setup(setup), m_noiseWindowLength(noiseWindowLength), m_maxTouchedPlans(NO_CUTVALUE), m_maxNumberOfHits(NO_CUTVALUE), m_vetoOnBIF(true)  
{
  if (m_setup.hasBIF()) setBIFparameters(m_setup.getBIF(),intervalle<int>(-8,-6)); //default interval corresponds to october 2016 GIF++ test beam
}

void RawHit_SDHCAL_Data_Reader_Noise::translateToEventTimeIntervalle(std::vector<unsigned int>& eventsTimes)
{
  if (m_readoutTimeDistribution.empty()) return;
  unsigned int startTime=m_readoutTimeDistribution.begin()->first;
  unsigned int endTime=m_readoutTimeDistribution.rbegin()->first;
  do
    {
      eventsTimes.push_back(startTime);
      startTime+=m_noiseWindowLength;
    } while (startTime+m_noiseWindowLength<endTime);
}

 bool RawHit_SDHCAL_Data_Reader_Noise::veto(const RawHit_SDHCAL_Data& d)
 {
   if (m_maxNumberOfHits<NO_CUTVALUE && d.getHitVector().size()>m_maxNumberOfHits) return true;
   if (m_vetoOnBIF && ( ! extract(d.getHitVector(),m_setup.getBIF(),rawHit_DIF()).empty()) ) return true;
   if (m_maxTouchedPlans<NO_CUTVALUE) 
     {
       std::set<unsigned int> plans;
       for (std::vector<RawCalorimeterHitPointer>::const_iterator itHit=d.getHitVector().begin(); itHit!=d.getHitVector().end(); ++itHit)
	 plans.insert(m_setup.getPlanNumber(itHit->dif()));
       if (plans.size()>m_maxTouchedPlans) return true;
     }
   return false;
 }
