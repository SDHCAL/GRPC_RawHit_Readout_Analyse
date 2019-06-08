#include "RawHit_SDHCAL_Data_Reader_Event_Filter.h"

#include <iostream>

RawHit_SDHCAL_Data_Reader_Event_Filter::~RawHit_SDHCAL_Data_Reader_Event_Filter()
{
  std::cout << "RawHit_SDHCAL_Data_Reader_Event_Filter have seen " <<  m_seenEvents << " events and kept " <<  m_keptEvents << " events." << std::endl;
  std::cout << " number of rejected events : " << std::endl; 
  for (auto &condpair :  m_RejectConditions)
    std::cout << condpair.first.first <<  "  has rejected : " << condpair.second << std::endl; 
}

void RawHit_SDHCAL_Data_Reader_Event_Filter::process(const RawHit_SDHCAL_Data& d)
{
  m_seenEvents++;
  for (auto &condpair :  m_RejectConditions)
    if (condpair.first.second->reject(d)) {++(condpair.second); return;}
  ++m_keptEvents;
  notifyListeners(d);
}

