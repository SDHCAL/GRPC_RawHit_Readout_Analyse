#include "RawHit_SDHCAL_Data_Reader_TriggeredMode.h"

#include "RawCalorimeterHitPointerTools.h"
#include "RawHit_SDHCAL_Data.h"


void RawHit_SDHCAL_Data_Reader_TriggeredMode::process(const RawHit_SDHCAL_Data& d)
{
  std::vector<RawCalorimeterHitPointer> event=extract(d.getHitVector(),m_timeWindow,rawHit_TimeStamp());
  RawHit_SDHCAL_Data dEvent(event,d);
  notifyListeners(dEvent);  
}
