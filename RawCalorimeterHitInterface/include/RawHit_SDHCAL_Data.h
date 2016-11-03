#ifndef RawHit_SDHCAL_Data_HH
#define RawHit_SDHCAL_Data_HH

#include "RawCalorimeterHitPointer.h"
#include <vector>
#include <map>
#include "EVENT/LCCollection.h"
#include "EVENT/LCParameters.h"

#include <stdint.h>

//Should we add Temperature info ?

struct DIF_timeInfo
{
  unsigned int DTC;
  unsigned int GTC;
  unsigned int BCID;
  uint64_t AbsBCID;
};

class RawHit_SDHCAL_Data
{
 public:
  RawHit_SDHCAL_Data(EVENT::LCCollection& col, int runNumber, int eventNumber, int64_t eventTimeStamp);
  RawHit_SDHCAL_Data(const std::vector<RawCalorimeterHitPointer>&vec, int runNumber, int eventNumber, int64_t eventTimeStamp);
  RawHit_SDHCAL_Data(const std::vector<RawCalorimeterHitPointer>&vec, const RawHit_SDHCAL_Data &d);
  
  void FillTimeInfo(const EVENT::LCParameters&);
  void replaceVec(const std::vector<RawCalorimeterHitPointer>&vec) {m_hitvec=vec;}

  int getRunNumber() const {return m_runNumber;}
  int getEventNumber() const {return m_eventNumber;}
  int64_t getEventTimeStamp() const {return m_eventTimeStamp;}
  const std::vector<RawCalorimeterHitPointer>& getHitVector() const {return m_hitvec;}
  std::map<unsigned int,DIF_timeInfo> DIFtimeInfo() const {return m_DIFtimeInfo;}
  const EVENT::LCParameters* getCollectionParameters() const {return m_originalCollectionParameters;}

 private:
  int m_runNumber;
  int m_eventNumber;
  int64_t m_eventTimeStamp;
  std::vector<RawCalorimeterHitPointer> m_hitvec;
  std::map<unsigned int,DIF_timeInfo> m_DIFtimeInfo;
  const EVENT::LCParameters *m_originalCollectionParameters;
};

#endif

