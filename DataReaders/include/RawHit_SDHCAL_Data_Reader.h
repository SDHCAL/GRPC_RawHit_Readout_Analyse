#ifndef RawHit_SDHCAL_Data_Reader_HH
#define RawHit_SDHCAL_Data_Reader_HH

#include "RawHit_SDHCAL_Data_Listener.h"

#include <set>

class RawHit_SDHCAL_Data_Reader
{
 public:
  void registerDataListener(RawHit_SDHCAL_Data_Listener &ls)
  {m_dataListeners.insert(m_dataListeners.end(), &ls);}
  void removeDataListener(RawHit_SDHCAL_Data_Listener &ls)
  {m_dataListeners.erase(m_dataListeners.find(&ls));}
  void notifyListeners(const RawHit_SDHCAL_Data& d)
  {
    for (std::set<RawHit_SDHCAL_Data_Listener*>::iterator it=m_dataListeners.begin();
	 it != m_dataListeners.end(); ++it) (*it)->process(d);
  }
 private:
  std::set<RawHit_SDHCAL_Data_Listener*> m_dataListeners;
};

#endif
