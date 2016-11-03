#include "RawHit_SDHCAL_Data_Reader_FromBIF.h"

#include "RawCalorimeterHitPointerTools.h"
#include "RawHit_SDHCAL_Data.h"

#include <iostream>

RawHit_SDHCAL_Data_Reader_FromBIF::~RawHit_SDHCAL_Data_Reader_FromBIF()
{
  std::cout << "RawHit_SDHCAL_Data_Reader_fromBIF has seen " << m_nBIFseen << " BIF triggers" << std::endl;
}


void RawHit_SDHCAL_Data_Reader_FromBIF::process(const RawHit_SDHCAL_Data& d)
{
  std::vector<RawCalorimeterHitPointer> BIF_hitvector=extract(d.getHitVector(),m_DIFnumber_of_the_BIF,rawHit_DIF());
  if (BIF_hitvector.empty()) return;
  RawHit_SDHCAL_Data dBIF(std::vector<RawCalorimeterHitPointer>(),d);
  for (std::vector<RawCalorimeterHitPointer>::iterator it=BIF_hitvector.begin(); it != BIF_hitvector.end(); ++it)
    {
      int intervalleLowerBound=m_timeWindowFromBIF.first+(*it)->getTimeStamp();
      if (intervalleLowerBound<0) intervalleLowerBound=0;
      if (0==intervalleLowerBound) { outOfTimeReadout.print(); continue;}
      int intervalleUpperBound=m_timeWindowFromBIF.second+(*it)->getTimeStamp();
      if (intervalleUpperBound<0) intervalleUpperBound=0; //should never happened
      UI_intervalle timeWindow((unsigned int)intervalleLowerBound,(unsigned int)intervalleUpperBound);
      std::vector<RawCalorimeterHitPointer> BIFevent=extract(d.getHitVector(),timeWindow,rawHit_TimeStamp());
      if (!(contains<unsigned int>((*it)->getTimeStamp())(timeWindow) ) )  BIFevent.push_back(*it); //add BIF signal if not there
      dBIF.replaceVec(BIFevent);
      notifyListeners(dBIF);
      ++m_nBIFseen;
    }
}
