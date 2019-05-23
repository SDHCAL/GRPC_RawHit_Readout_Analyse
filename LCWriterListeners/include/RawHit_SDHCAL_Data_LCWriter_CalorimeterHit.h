#ifndef RawHit_SDHCAL_Data_LCWriter_CalorimeterHit_HH
#define RawHit_SDHCAL_Data_LCWriter_CalorimeterHit_HH

#include "RawHit_SDHCAL_Data_LCWriter.h"
#include "ExperimentalSetup.h"
#include <map>
#include <vector>
#include <string>

class RawHit_SDHCAL_Data_LCWriter_CalorimeterHit : public RawHit_SDHCAL_Data_LCWriter
{
 public:
  RawHit_SDHCAL_Data_LCWriter_CalorimeterHit(ExperimentalSetup& setup,std::string collectionName="DHCAL_Hits");
 private:
  virtual IMPL::LCCollectionVec* createAndFillCollection(const RawHit_SDHCAL_Data&);
  virtual void finalizeCollection(IMPL::LCCollectionVec*);

  ExperimentalSetup *m_setup;
  std::map<std::string, std::vector<int> > m_parametersFromSetup;
  std::vector<unsigned int> m_tricot_DIFs;
};

#endif
