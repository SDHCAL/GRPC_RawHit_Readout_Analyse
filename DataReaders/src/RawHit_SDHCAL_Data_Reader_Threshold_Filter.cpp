#include "RawHit_SDHCAL_Data_Reader_Threshold_Filter.h"

#include "domain.h"
#include "RawCalorimeterHitPointerTools.h"
#include "RawHit_SDHCAL_Data.h"

void RawHit_SDHCAL_Data_Reader_Threshold_Filter::process(const RawHit_SDHCAL_Data &d)
{
  if (!m_exclusive && m_thresholdLevel<=1) {notifyListeners(d); return;}
  RawHit_SDHCAL_Data dThresh(std::vector<RawCalorimeterHitPointer>(),d);
  if (m_exclusive) dThresh.replaceVec( extract(d.getHitVector(), m_thresholdLevel, rawHit_Threshold(), m_notInverted) );
  else 
    {
      if (m_thresholdLevel==2) dThresh.replaceVec( extract(d.getHitVector(),aboveSecondThreshold() , rawHit_Threshold(), m_notInverted) );
      if (m_thresholdLevel==3) dThresh.replaceVec( extract(d.getHitVector(),aboveThirdThreshold() , rawHit_Threshold(), m_notInverted) ); 
    }
  notifyListeners(dThresh);
}
