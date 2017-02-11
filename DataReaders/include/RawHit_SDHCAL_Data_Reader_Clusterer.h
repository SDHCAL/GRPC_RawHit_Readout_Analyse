#ifndef RawHit_SDHCAL_Data_Reader_Clusterer_HH
#define RawHit_SDHCAL_Data_Reader_Clusterer_HH

#include "RawHit_SDHCAL_Data.h"
#include "RawHit_SDHCAL_Data_Listener.h"
#include "RawHit_SDHCAL_Data_Reader.h"

#include "TimeCounter.h"

template <class mergePredicate>
class RawHit_SDHCAL_Data_Reader_Clusterer : public RawHit_SDHCAL_Data_Listener, public RawHit_SDHCAL_Data_Reader
{
 public:
 RawHit_SDHCAL_Data_Reader_Clusterer(mergePredicate &f,bool verbose=false) : m_f(f), m_verbose(verbose), m_nevent(0) {}
  void process(const RawHit_SDHCAL_Data& d)
  {
    TimeCounters["CopyVec"].start(); RawHit_SDHCAL_Data newd(d.getHitVector(), d, d.getNumberOfEventInThisData()); TimeCounters["CopyVec"].stop();//not terrific in terms of copy
    TimeCounters["Clus"].start(); newd.clusterize(m_f); TimeCounters["Clus"].stop(); 
    if (m_verbose) std::cout << ++m_nevent << " : " << newd.getHitVector().size() << " hits clustered in " << newd.getClusters().size() << " clusters.\r"; std::cout.flush();
    TimeCounters["Listen"].start(); notifyListeners(newd); TimeCounters["Listen"].stop();
  }
 private:
  mergePredicate &m_f;
  bool m_verbose;
  unsigned int m_nevent;
};

#endif
