#include "CMSTree_Writer_NoBIF.h"

#include "TTree.h"
#include "RawCalorimeterHitPointerTools.h"

#include <iostream>

void CMSTree_Writer_NoBIF::process(const RawHit_SDHCAL_Data& d) 
{
  ++EventCount;
  if (d.getHitVector().empty())
    {
      if (! m_addEntryForEventsWithNoHits) return; 
      BIFHITGTC=0;
      BIFHITts=0;
    }
  else
    {
      //find minimum time stamp to set equivalent BIF time info
      int itime=0;
      for (int i=1; i<d.getHitVector().size(); ++i)
	if (d.getHitVector()[i]->getTimeStamp()<d.getHitVector()[itime]->getTimeStamp())
	  itime=i;
      BIFHITGTC=d.DIFtimeInfo()[d.getHitVector()[itime].dif()].GTC;
      BIFHITts=d.getHitVector()[itime]->getTimeStamp();
    }
  processHits( d.getHitVector() );
  getTree()->Fill();
  if (0==(EventCount%1000))
    { std::cout << "CMSTree_Writer_NoBIF " << EventCount << " events in TTree. \r"; std::cout.flush();}
}

