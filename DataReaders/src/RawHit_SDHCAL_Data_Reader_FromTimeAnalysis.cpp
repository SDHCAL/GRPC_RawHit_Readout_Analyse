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


void RawHit_SDHCAL_Data_Reader_FromTimeAnalysis::BIFveto(std::list<unsigned int>& eventsTimes, const RawHit_SDHCAL_Data& d)
{
  if (! m_vetoOnBIF) return;
  std::list<unsigned int>::iterator it=eventsTimes.begin();
  while (it!= eventsTimes.end())
    {
      bool toDelete=false;
      if (m_BIFtimeWindow.first+int(*it)+m_SelectEventTimeWindow.first<0 || m_BIFtimeWindow.second+int(*it)+m_SelectEventTimeWindow.second<0) toDelete=true;
      else 
	{
	  UI_intervalle BIF_timeWindow_to_search((*it)+m_SelectEventTimeWindow.first+m_BIFtimeWindow.first,(*it)+m_SelectEventTimeWindow.second+m_BIFtimeWindow.second);
	  if (std::count_if(BIF_hitvector.begin(), BIF_hitvector.end(), rawHit_IsInIntervalle<rawHit_TimeStamp>(BIF_timeWindow_to_search))>0) toDelete=true;
	}
      if (toDelete) it=eventsTimes.erase(it);
      else ++it;
    }
}


void RawHit_SDHCAL_Data_Reader_FromTimeAnalysis::process(const RawHit_SDHCAL_Data& d)
{
  //std::cout << "RawHit_SDHCAL_Data_Reader_FromTimeAnalysis called " << std::endl;

  BIF_hitvector=extract(d.getHitVector(),m_DIFnumber_of_the_BIF,rawHit_DIF());
  FillReadoutTimeDistribution(d);

  std::list<unsigned int> eventTimes;
  translateToEventTimeIntervalle(eventTimes,d);
  BIFveto(eventTimes,d);

  while (eventTimes.size()>0 && *(eventTimes.begin())+m_SelectEventTimeWindow.first <0)
    {
      outOfTimeReadout.print();
      eventTimes.pop_front();
    }
  //std::cout << "RawHit_SDHCAL_Data_Reader_FromTimeAnalysis eventTimes size is  " << eventTimes.size() << std::endl;

  std::vector<RawCalorimeterHitPointer> eventHits;
  if (m_splitEventForListeners)
    for (std::list<unsigned int>::iterator itTime=eventTimes.begin(); itTime!=eventTimes.end(); ++itTime)
      {
	int intervalleLowerBound=m_SelectEventTimeWindow.first+ (*itTime);
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
	    ++m_nEventSeen;
	    RawHit_SDHCAL_Data eventData(eventHits,d);
	    notifyListeners(eventData);
	  }
      } 
  else
    {
      UI_domain timeDomains;
      for (std::list<unsigned int>::iterator itTime=eventTimes.begin(); itTime!=eventTimes.end(); ++itTime)
	timeDomains.addIntervalle(UI_intervalle(m_SelectEventTimeWindow.first+(*itTime),m_SelectEventTimeWindow.second+(*itTime)));
      std::vector<RawCalorimeterHitPointer> eventHits=extract(d.getHitVector(),timeDomains,rawHit_TimeStamp());
      m_nEventSeen+=eventTimes.size();
      RawHit_SDHCAL_Data eventData(eventHits,d,eventTimes.size());
      notifyListeners(eventData);
    }    
}
