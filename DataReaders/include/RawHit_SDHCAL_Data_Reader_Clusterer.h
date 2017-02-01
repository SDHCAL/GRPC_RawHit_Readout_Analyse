#ifndef RawHit_SDHCAL_Data_Reader_Clusterer_HH
#define RawHit_SDHCAL_Data_Reader_Clusterer_HH

#include "RawHit_SDHCAL_Data.h"
#include "RawHit_SDHCAL_Data_Listener.h"
#include "RawHit_SDHCAL_Data_Reader.h"

template <class mergePredicate>
class RawHit_SDHCAL_Data_Reader_Clusterer : public RawHit_SDHCAL_Data_Listener, public RawHit_SDHCAL_Data_Reader
{
 public:
 RawHit_SDHCAL_Data_Reader_Clusterer(mergePredicate &f) : m_f(f) {}
  void process(const RawHit_SDHCAL_Data& d)
  {
    RawHit_SDHCAL_Data newd(d.getHitVector(), d, d.getNumberOfEventInThisData()); //not terrific in terms of copy
    newd.clusterize(m_f);
    notifyListeners(newd);
  }
 private:
  mergePredicate &m_f;
};

#endif
