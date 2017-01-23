#include "CMSTree_Writer_NoBIF.h"

#include "TTree.h"
#include "RawCalorimeterHitPointerTools.h"

#include <iostream>

void CMSTree_Writer_NoBIF::process(const RawHit_SDHCAL_Data& d) 
{
  ++EventCount;
  if (! m_addEntryForEventsWithNoHits) return; 
  BIFHITGTC=d.DIFtimeInfo().begin()->second.GTC;
  BIFHITts=d.getFrameSubSet_intervalle_startTime();
  processHits( d.getHitVector() );
  getTree()->Fill();
  if (0==(EventCount%1000))
    { std::cout << "CMSTree_Writer_NoBIF " << EventCount << " events in TTree. \r"; std::cout.flush();}
}

