#include "Time_Decorator_For_RawHit_SDHCAL_Data_Listener.h"

Time_Decorator_For_RawHit_SDHCAL_Data_Listener::Time_Decorator_For_RawHit_SDHCAL_Data_Listener(RawHit_SDHCAL_Data_Listener& decoratee, std::string name) : m_decoratee(decoratee), m_counter(TimeCounters[std::string("Listener_")+name]) {;}

void Time_Decorator_For_RawHit_SDHCAL_Data_Listener::process(const RawHit_SDHCAL_Data& d)
{
  m_counter.start();
  m_decoratee.process(d);
  m_counter.stop();
}
