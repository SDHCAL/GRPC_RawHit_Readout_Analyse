#ifndef RawHit_SDHCAL_Data_HH
#define RawHit_SDHCAL_Data_HH

#include "RawCalorimeterHitPointer.h"
#include <vector>
#include <map>
#include "EVENT/LCCollection.h"
#include "EVENT/LCParameters.h"
#include "RawCalorimeterHitClustering.h"

#include "Clustering_bis.h" //test

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
  RawHit_SDHCAL_Data(const std::vector<RawCalorimeterHitPointer>&vec, int runNumber, int eventNumber, int64_t eventTimeStamp, int numberOfEventInThisData=1);
  RawHit_SDHCAL_Data(const std::vector<RawCalorimeterHitPointer>&vec, const RawHit_SDHCAL_Data &d, int numberOfEventInThisData=1);
  RawHit_SDHCAL_Data(const RawHitClustersList &clusters, const RawHit_SDHCAL_Data &d, int numberOfEventInThisData=1);
  
  void FillTimeInfo(const EVENT::LCParameters&);
  void replaceVec(const std::vector<RawCalorimeterHitPointer>&vec, int numberOfEventInThisData=1) {m_hitvec=vec; m_numberOfEventInThisData=numberOfEventInThisData;}
  void appendVec(const std::vector<RawCalorimeterHitPointer>&vec, int numberOfEventInThisData=1) {m_hitvec.insert(m_hitvec.end(),vec.begin(),vec.end()); m_numberOfEventInThisData+=numberOfEventInThisData;}

  int getRunNumber() const {return m_runNumber;}
  int getEventNumber() const {return m_eventNumber;}
  int getNumberOfEventInThisData() const {return m_numberOfEventInThisData;}
  int64_t getEventTimeStamp() const {return m_eventTimeStamp;}
  unsigned int getFrameSubSet_intervalle_startTime() const {return m_frameSubSet_intervalle_startTime;}
  void setFrameSubSet_intervalle_startTime(unsigned int val) {m_frameSubSet_intervalle_startTime=val;}
  const std::vector<RawCalorimeterHitPointer>& getHitVector() const {return m_hitvec;}
  std::map<unsigned int,DIF_timeInfo> DIFtimeInfo() const {return m_DIFtimeInfo;}
  const EVENT::LCParameters* getCollectionParameters() const {return m_originalCollectionParameters;}


  //cluster stuff
  bool hasCluster() const {return not m_clusters.empty();}
  const RawHitClustersList& getClusters() const {return m_clusters;}
  template <class mergePred>
    void clusterize(mergePred f)
    {
      m_clusters.clear();
      //SDHCAL::convertToClusterList(m_hitvec.begin(),m_hitvec.end(),m_clusters);
      //SDHCAL::clusterize(m_clusters,f);
      SDHCAL::clusterize(m_hitvec.begin(),m_hitvec.end(),m_clusters,f);
    }
  void clusterizeDefault() {clusterize(defaultRawHitMerge());}
 private:
  int m_runNumber;
  int m_eventNumber;
  int m_numberOfEventInThisData;
  int64_t m_eventTimeStamp;
  unsigned int m_frameSubSet_intervalle_startTime;
  std::vector<RawCalorimeterHitPointer> m_hitvec;
  std::map<unsigned int,DIF_timeInfo> m_DIFtimeInfo;
  const EVENT::LCParameters *m_originalCollectionParameters;
  RawHitClustersList m_clusters;
};

#endif

