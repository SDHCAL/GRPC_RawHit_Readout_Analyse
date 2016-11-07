#include "RawHit_SDHCAL_Data_Reader_FromTimeAnalysis.h"

#include <iostream>
#include <algorithm>
#include "RawHit_SDHCAL_Data.h"
#include "RawCalorimeterHitPointerTools.h"

RawHit_SDHCAL_Data_Reader_FromTimeAnalysis::~RawHit_SDHCAL_Data_Reader_FromTimeAnalysis()
{
  std::cout << " RawHit_SDHCAL_Data_Reader_fromTimeAnalysis " << m_nEventSeen << "  events seen  and " << m_nHitWithNegativeTimeStampSeen << " hits discarded due to negative time stamp " << std::endl;
}

void RawHit_SDHCAL_Data_Reader_FromTimeAnalysis::FillReadoutTimeDistribution(const RawHit_SDHCAL_Data& d)
{
  m_readoutTimeDistribution.clear();
  for (std::vector<RawCalorimeterHitPointer>::const_iterator itHit=d.getHitVector().begin(); itHit!= d.getHitVector().end(); ++itHit)
    {
      if (hasBIFnumber() && m_DIFnumber_of_the_BIF==itHit->dif()) continue;
      if ((*itHit)->getTimeStamp()<0) {++m_nHitWithNegativeTimeStampSeen; if (m_discardNegativeTimeStamp) continue;}
      ++m_readoutTimeDistribution[(unsigned int)(*itHit)->getTimeStamp()];
    }
}

void RawHit_SDHCAL_Data_Reader_FromTimeAnalysis::process(const RawHit_SDHCAL_Data& d)
{
  RawHit_SDHCAL_Data eventData(std::vector<RawCalorimeterHitPointer>(),d);
  std::vector<RawCalorimeterHitPointer> eventHits;
  FillReadoutTimeDistribution(d);
  std::vector<unsigned int> eventTimes;
  translateToEventTimeIntervalle(eventTimes);
  std::vector<RawCalorimeterHitPointer> BIF_hitvector=extract(d.getHitVector(),m_DIFnumber_of_the_BIF,rawHit_DIF());
  for (std::vector<unsigned int>::iterator itTime=eventTimes.begin(); itTime!=eventTimes.end(); ++itTime)
    {
      int intervalleLowerBound=m_SelectEventTimeWindow.first+(*itTime);
      if (intervalleLowerBound<0) {outOfTimeReadout.print(); continue;}
      std::vector<RawCalorimeterHitPointer> eventHits=extract(d.getHitVector(),UI_intervalle(intervalleLowerBound,m_SelectEventTimeWindow.second+(*itTime)),rawHit_TimeStamp());
      if (hasBIFnumber())
	{
	  eventHits.erase(std::remove_if(eventHits.begin(),eventHits.end(), rawHit_IsEqual<rawHit_DIF>(m_DIFnumber_of_the_BIF) ),eventHits.end());//remove BIF data
	  int BIFintervalleLowerBound=m_BIFtimeWindow.first+intervalleLowerBound;
	  if (BIFintervalleLowerBound<0)
	    {
	      outOfTimeBIF.print();
	      if (m_skipIfBIFisOutsideReadout) continue;
	    }
	  std::vector<RawCalorimeterHitPointer> BIFhits=extract(BIF_hitvector,UI_intervalle(BIFintervalleLowerBound,m_BIFtimeWindow.second+m_SelectEventTimeWindow.second),rawHit_TimeStamp());
	  eventHits.insert(eventHits.end(),BIFhits.begin(),BIFhits.end());
	}
      RawHit_SDHCAL_Data thisEventData(eventHits,eventData);
      if (veto(eventData)) continue;
      ++m_nEventSeen;
      if (m_splitEventForListeners) notifyListeners(thisEventData);
      else eventData.appendVec(eventHits);
    }
  if (!m_splitEventForListeners) notifyListeners(eventData);
}
