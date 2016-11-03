#ifndef RawHit_SDHCAL_Data_LCWriter_RawCalorimeterHit_HH
#define RawHit_SDHCAL_Data_LCWriter_RawCalorimeterHit_HH

#include "RawHit_SDHCAL_Data_LCWriter.h"

class RawHit_SDHCAL_Data_LCWriter_RawCalorimeterHit : public RawHit_SDHCAL_Data_LCWriter
{
 public:
 RawHit_SDHCAL_Data_LCWriter_RawCalorimeterHit(std::string collectionName="DHCALRawHits") : RawHit_SDHCAL_Data_LCWriter(collectionName) {;}
 private:
  virtual IMPL::LCCollectionVec* createAndFillCollection(const RawHit_SDHCAL_Data&);
  virtual void finalizeCollection(IMPL::LCCollectionVec*);

};

#endif
