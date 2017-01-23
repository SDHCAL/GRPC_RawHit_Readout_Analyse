#include "RawHit_SDHCAL_Data_Reader_Noise.h"

#include "RawHit_SDHCAL_Data.h"
#include <set>

const unsigned int RawHit_SDHCAL_Data_Reader_Noise::NO_CUTVALUE=10000000;

RawHit_SDHCAL_Data_Reader_Noise::RawHit_SDHCAL_Data_Reader_Noise(ExperimentalSetup& setup, unsigned int noiseWindowLength) 
  : RawHit_SDHCAL_Data_Reader_FromTimeAnalysis(UI_intervalle(0,noiseWindowLength-1)),
    m_setup(setup), m_noiseWindowLength(noiseWindowLength), m_maxTouchedPlans(NO_CUTVALUE), m_maxNumberOfHits(NO_CUTVALUE) 
{
  if (m_setup.hasBIF()) setBIFparameters(m_setup.getBIF(),intervalle<int>(-8,-6)); //default interval corresponds to october 2016 GIF++ test beam
  setSplitEventForListeners(false);
  setVetoOnBIF();
}

void RawHit_SDHCAL_Data_Reader_Noise::translateToEventTimeIntervalle(std::list<unsigned int>& eventsTimes, const RawHit_SDHCAL_Data& d)
{
  if (m_readoutTimeDistribution.empty()) return;
  unsigned int startTime=m_readoutTimeDistribution.begin()->first;
  unsigned int endTime=m_readoutTimeDistribution.rbegin()->first;
  do
    {
      if (! veto(UI_intervalle(startTime,startTime+m_noiseWindowLength-1),d) ) eventsTimes.push_back(startTime);
      startTime+=m_noiseWindowLength;
    } while (startTime+m_noiseWindowLength<endTime);
}

 

 bool RawHit_SDHCAL_Data_Reader_Noise::vetoPlans(UI_intervalle timeWin, const RawHit_SDHCAL_Data& d)
 {
   if (m_maxTouchedPlans==NO_CUTVALUE) return false;
   std::set<unsigned int> plans;
   std::vector<RawCalorimeterHitPointer> data=extract(d.getHitVector(),timeWin,rawHit_TimeStamp()); //to be optimized
   for (std::vector<RawCalorimeterHitPointer>::const_iterator itHit=data.begin(); itHit!=data.end(); ++itHit)
     plans.insert(m_setup.getPlanNumber(itHit->dif()));
   if (plans.size()>m_maxTouchedPlans) return true;
   return false;
 }
