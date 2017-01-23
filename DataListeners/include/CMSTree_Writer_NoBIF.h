#ifndef CMSTree_Writer_NoBIF_HH
#define CMSTree_Writer_NoBIF_HH

#include "CMSTree_Writer.h"

class CMSTree_Writer_NoBIF : public CMSTree_Writer
{
 public:
 CMSTree_Writer_NoBIF(ExperimentalSetup& setup,unsigned int halfdecalage) : CMSTree_Writer(setup,halfdecalage), m_addEntryForEventsWithNoHits(false) {}

  void process(const RawHit_SDHCAL_Data&);

  void setAddEntryForEventsWithNoHits(bool val=true) {m_addEntryForEventsWithNoHits=val;}

 private:
  bool m_addEntryForEventsWithNoHits;
};

#endif
