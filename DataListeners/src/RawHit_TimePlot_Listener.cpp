#include "RawHit_TimePlot_Listener.h"

#include "RawHit_SDHCAL_Data.h"

RawHit_TimePlot_Listener::RawHit_TimePlot_Listener(ExperimentalSetup& setup) : m_setup(setup), m_negativeTimeStampCounters(&setup) {;}

void RawHit_TimePlot_Listener::printReport(std::ostream& oflux)
{
  oflux << "Report on negative time stamp :" << std::endl;
  m_negativeTimeStampCounters.write(oflux);
}

void RawHit_TimePlot_Listener::saveTo(TDirectory* ROOTdir)
{
  if (NULL==ROOTdir) return;
  m_negativeTimeStampCounters.saveTo(ROOTdir,"NegativeTimeStamp",false);
}

void RawHit_TimePlot_Listener::process(const RawHit_SDHCAL_Data& d)
{
  for (auto &hit : d.getHitVector())
    if (hit->getTimeStamp()<0)
      m_negativeTimeStampCounters.add(1,hit.dif(),hit.asic(),hit.channel());
  m_negativeTimeStampCounters.newSet();
}
