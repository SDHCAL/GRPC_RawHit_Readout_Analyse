#include "RawHit_SDHCAL_Data_Reader_FromTimeAnalysis.h"

#include <iostream>
#include <algorithm>
#include "RawHit_SDHCAL_Data.h"
#include "RawCalorimeterHitPointerTools.h"

RawHit_SDHCAL_Data_Reader_FromTimeAnalysis::~RawHit_SDHCAL_Data_Reader_FromTimeAnalysis()
{
  std::cout << " RawHit_SDHCAL_Data_Reader_fromTimeAnalysis " << m_nEventSeen << "  events seen  and " << m_nHitWithNegativeTimeStampSeen << " hits discarded due to negative time stamp. Maximum was set to  " << m_maxEventsToSend << std::endl;
}


void RawHit_SDHCAL_Data_Reader_FromTimeAnalysis::FillReadoutTimeDistribution(const RawHit_SDHCAL_Data& d)
{
  m_readoutTimeDistribution.clear();
  for (std::vector<RawCalorimeterHitPointer>::const_iterator itHit=d.getHitVector().begin(); itHit!= d.getHitVector().end(); ++itHit)
    {
      if (hasBIFnumber() && m_BIFtimeWindow_per_BIF_DIFnumber.count(itHit->dif())==1) continue;
      if ((*itHit)->getTimeStamp()<0) {++m_nHitWithNegativeTimeStampSeen; if (m_discardNegativeTimeStamp) continue;}
      ++m_readoutTimeDistribution[(unsigned int)(*itHit)->getTimeStamp()];
    }
  //for (std::map<unsigned int,unsigned int>::iterator it=m_readoutTimeDistribution.begin(); it !=m_readoutTimeDistribution.end(); ++it)
  //  std::cout << "("<<it->first<<":"<<it->second<<") ";
  //std::cout << std::endl;
}


void RawHit_SDHCAL_Data_Reader_FromTimeAnalysis::BIFveto(std::list<unsigned int>& eventsTimes, const RawHit_SDHCAL_Data& d)
{
  if (! m_vetoOnBIF) return;
  std::list<unsigned int>::iterator it=eventsTimes.begin();
  while (it!= eventsTimes.end())
    {
      bool toDelete=false;
      for (std::map<unsigned int, intervalle<int> >::iterator BIFtimeWindows_iterator= m_BIFtimeWindow_per_BIF_DIFnumber.begin(); BIFtimeWindows_iterator != m_BIFtimeWindow_per_BIF_DIFnumber.end(); ++BIFtimeWindows_iterator)
	{
	  const unsigned int &BIF_number=BIFtimeWindows_iterator->first;
	  intervalle<int> &timeWindow=BIFtimeWindows_iterator->second;
	  if (timeWindow.first+int(*it)+m_SelectEventTimeWindow.first<0 || timeWindow.second+int(*it)+m_SelectEventTimeWindow.second<0) toDelete=true;
	  else 
	    {
	      UI_intervalle BIF_timeWindow_to_search((*it)+m_SelectEventTimeWindow.first+timeWindow.first,(*it)+m_SelectEventTimeWindow.second+timeWindow.second);
	      if (std::count_if(BIF_hitvector_per_BIF_DIFnumber[BIF_number].begin(), BIF_hitvector_per_BIF_DIFnumber[BIF_number].end(), rawHit_IsInIntervalle<rawHit_TimeStamp>(BIF_timeWindow_to_search))>0) toDelete=true;
	    }
	  if (toDelete) break;
	}
      if (toDelete) it=eventsTimes.erase(it);
      else ++it;
    }
}


void RawHit_SDHCAL_Data_Reader_FromTimeAnalysis::process(const RawHit_SDHCAL_Data& d)
{
  //std::cout << "RawHit_SDHCAL_Data_Reader_FromTimeAnalysis called " << std::endl;

  if (m_nEventSeen>=m_maxEventsToSend) {tooMuchEvent.print(); return;}

  
  for (std::map<unsigned int, intervalle<int> >::iterator BIFtimeWindows_iterator= m_BIFtimeWindow_per_BIF_DIFnumber.begin(); BIFtimeWindows_iterator != m_BIFtimeWindow_per_BIF_DIFnumber.end(); ++BIFtimeWindows_iterator)
    {
      BIF_hitvector_per_BIF_DIFnumber[BIFtimeWindows_iterator->first]=extract(d.getHitVector(),BIFtimeWindows_iterator->first,rawHit_DIF());
    }
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

  UI_domain timeDomains;
  for (std::list<unsigned int>::iterator itTime=eventTimes.begin(); itTime!=eventTimes.end(); ++itTime)
    timeDomains.addIntervalle(UI_intervalle(m_SelectEventTimeWindow.first+(*itTime),m_SelectEventTimeWindow.second+(*itTime)));
  std::vector<RawCalorimeterHitPointer> eventHits_all=extract(d.getHitVector(),timeDomains,rawHit_TimeStamp());
  if (m_splitEventForListeners)
    {
      for (std::list<unsigned int>::iterator itTime=eventTimes.begin(); itTime!=eventTimes.end(); ++itTime)
	{
	  int intervalleLowerBound=m_SelectEventTimeWindow.first+ (*itTime);
	  std::vector<RawCalorimeterHitPointer> eventHits=extract(eventHits_all,UI_intervalle(intervalleLowerBound,m_SelectEventTimeWindow.second+(*itTime)),rawHit_TimeStamp());
	  if (hasBIFnumber() && ! eventHits.empty()) // il y a au moins une BIF
	    {
	      bool skip_this_eventTimes=false;
	      for (std::map<unsigned int, intervalle<int> >::iterator BIFtimeWindows_iterator= m_BIFtimeWindow_per_BIF_DIFnumber.begin();
		   BIFtimeWindows_iterator != m_BIFtimeWindow_per_BIF_DIFnumber.end(); ++BIFtimeWindows_iterator)
		{
		  const unsigned int &BIF_number=BIFtimeWindows_iterator->first;
		  intervalle<int> &BIFtimeWindow=BIFtimeWindows_iterator->second;

		  eventHits.erase(std::remove_if(eventHits.begin(),eventHits.end(), rawHit_IsEqual<rawHit_DIF>( BIF_number) ),eventHits.end());//remove BIF data
		  int BIFintervalleLowerBound=BIFtimeWindow.first+intervalleLowerBound;
		  if (BIFintervalleLowerBound<0)
		    {
		      outOfTimeBIF.print();
		      if (m_skipIfBIFisOutsideReadout) {skip_this_eventTimes=true;break;} // skip cet  eventTimes, sort de la boucle sur les BIF  
		    }
		  std::vector<RawCalorimeterHitPointer> BIFhits=extract(BIF_hitvector_per_BIF_DIFnumber[BIF_number],UI_intervalle(BIFintervalleLowerBound,BIFtimeWindow.second+m_SelectEventTimeWindow.second+(*itTime)),rawHit_TimeStamp()); //ajoute les hits BIF ayant le bon intervalle
		  eventHits.insert(eventHits.end(),BIFhits.begin(),BIFhits.end());
		}
	      if (skip_this_eventTimes) continue;
	    }
	  ++m_nEventSeen;
	  RawHit_SDHCAL_Data eventData(eventHits,d);
	  eventData.setFrameSubSet_intervalle_startTime(intervalleLowerBound);
	  notifyListeners(eventData);
	}
    }
  else
    {
      std::vector<RawCalorimeterHitPointer>& eventHits=eventHits_all;
      m_nEventSeen+=eventTimes.size();
      RawHit_SDHCAL_Data eventData(eventHits,d,eventTimes.size());
      if (! eventTimes.empty())
	eventData.setFrameSubSet_intervalle_startTime(m_SelectEventTimeWindow.first+(*eventTimes.begin()));
      notifyListeners(eventData);
    }    
}
