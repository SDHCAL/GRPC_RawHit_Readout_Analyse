#include "RawHit_SDHCAL_Data_Reader_FromClusters.h"


#include "RawHit_SDHCAL_Data.h"
#include "RawCalorimeterHitUtilities.h"
#include "RawCalorimeterHitPointerExperimentalSetupTools.h"
#include "RawCalorimeterHitPointerTools.h"

void RawHit_SDHCAL_Data_Reader_FromClusters::process(const RawHit_SDHCAL_Data &d)
{
  //check if clusters have been computed
  if (not d.getHitVector().empty() && d.getClusters().empty())
    return;
  std::vector<IMPL::RawCalorimeterHitImpl*> rawHitVector;
  std::vector<RawCalorimeterHitPointer> rawHitPointerVec;
  for (std::list<RawHitCluster>::const_iterator itcl=d.getClusters().begin(); itcl != d.getClusters().end(); ++itcl)
    {
      if (itcl->empty()) continue;
      std::map<unsigned int, unsigned int> planList=distribution(*itcl,rawHit_PlanNumber(m_setup));
      if (planList.count(BaseExperimentalSetupGetter::ERRORCODE) > 0) continue;
      unsigned int plan=mostFrequentValue(planList);
      unsigned int IorStrip=int(mean(*itcl,rawHit_IorStrip(m_setup)));
      unsigned int JorGapp=int(mean(*itcl,rawHit_JorGap(m_setup)));
      unsigned int dif,asic,channel;
      m_setup.getDAQ_ID(IorStrip,JorGapp,plan,dif,asic,channel);
      unsigned int timeStamp=int(mean<rawHit_TimeStamp>(*itcl));
      std::map<unsigned int, unsigned int> thresholds=distribution<rawHit_Amplitude>(*itcl);
      unsigned int amplitude=thresholds.rbegin()->first;
      IMPL::RawCalorimeterHitImpl* newRawHit=createRawHit(amplitude, timeStamp, dif, asic, channel);
      rawHitVector.push_back(newRawHit);
      rawHitPointerVec.push_back(RawCalorimeterHitPointer(newRawHit));
    }
  RawHit_SDHCAL_Data newd(rawHitPointerVec,d,d.getNumberOfEventInThisData());
  notifyListeners(newd);

  //clean stuff
  for (std::vector<IMPL::RawCalorimeterHitImpl*>::iterator it=rawHitVector.begin(); it != rawHitVector.end(); ++it)
    delete (*it);
}
