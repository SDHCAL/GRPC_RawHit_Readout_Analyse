#include "RawHit_SDHCAL_Data_Reader_Trivent.h"

#include <stdlib.h>
#include <iterator>     // std::distance

void RawHit_SDHCAL_Data_Reader_Trivent::translateToEventTimeIntervalle(std::list<unsigned int>& eventsTimes, const RawHit_SDHCAL_Data&)
{
  if (m_ultraVerboseDebugOutput) printData("TRIVENT DEBUG : received ");
  bool eraseFirst=false;
  bool nextHasBeenErased=false;
  for (std::map<unsigned int,unsigned int>::iterator it=m_readoutTimeDistribution.begin(); it!= m_readoutTimeDistribution.end(); ++it) 
    {
      if (m_ultraVerboseDebugOutput) std::cout << std::distance(m_readoutTimeDistribution.begin(),it) << "/" << m_readoutTimeDistribution.size() << "(" << int(nextHasBeenErased) << ")" << std::endl; 
      if (nextHasBeenErased) --it;
      nextHasBeenErased=false;
      bool eraseIt=(it->second<m_noiseCut);
      if (!eraseIt) 
	{
	  std::map<unsigned int,unsigned int>::iterator itnext=it;
	  ++itnext;
	  if (itnext==m_readoutTimeDistribution.end()) continue;
	  if (itnext->first<it->first) {std::cout<< "TRIVENT GOT LOST : " << itnext->first << " at " << &(*itnext) << " and " << it->first << " at " << &(*it) << std::endl; printData("TRIVENT DEBUG : time distribution current status "); abort();}
	  if (itnext->first-it->first<=m_timeWin)
	    {
	      if (itnext->second >= it->second) eraseIt=true;
	      else {m_readoutTimeDistribution.erase(itnext); nextHasBeenErased=true; }
	    }	  
	}
      if (eraseIt)
	{
	  if (it == m_readoutTimeDistribution.begin()) eraseFirst=true;
	  else
	    {
	      std::map<unsigned int,unsigned int>::iterator itprev=it; 
	      --itprev;
	      m_readoutTimeDistribution.erase(it);
	       it=itprev;
	    }
	}
    }
  if (eraseFirst) m_readoutTimeDistribution.erase(m_readoutTimeDistribution.begin());
  
  if (m_readoutTimeDistribution.empty()) return;
  eventsTimes.push_back(m_readoutTimeDistribution.begin()->first);
  for (std::map<unsigned int,unsigned int>::iterator it=m_readoutTimeDistribution.begin(); it!= m_readoutTimeDistribution.end(); ++it) 
    if (it->first>(*eventsTimes.rbegin()+2*m_timeWin)) eventsTimes.push_back(it->first);
}


void RawHit_SDHCAL_Data_Reader_Trivent::printData(const char* message)
{
  std::cout << message;
  for (auto &m : m_readoutTimeDistribution) std::cout << " (" << m.first << ":"<<m.second<<") ";
  std::cout << std::endl;
}
