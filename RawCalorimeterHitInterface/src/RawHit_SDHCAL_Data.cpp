#include "RawHit_SDHCAL_Data.h"

#include <stdlib.h>



RawHit_SDHCAL_Data::RawHit_SDHCAL_Data(EVENT::LCCollection& col, int runNumber, int eventNumber, int64_t eventTimeStamp) 
  : m_runNumber(runNumber),
    m_eventNumber(eventNumber),
    m_numberOfEventInThisData(1),
    m_eventTimeStamp(eventTimeStamp),
    m_frameSubSet_intervalle_startTime(0),
    m_buildClusterVec(false)
{
  m_originalCollectionParameters=&col.getParameters();
  FillTimeInfo(col.getParameters());
  int nHits = col.getNumberOfElements();
  for(int i=0 ; i<nHits ; ++i)
    {
      EVENT::RawCalorimeterHit* mh =  (EVENT::RawCalorimeterHit*) col.getElementAt(i) ;
      m_hitvec.push_back(RawCalorimeterHitPointer(mh));
    }
}

RawHit_SDHCAL_Data::RawHit_SDHCAL_Data(const std::vector<RawCalorimeterHitPointer>&vec, int runNumber, int eventNumber, int64_t eventTimeStamp, int numberOfEventInThisData)
 : m_runNumber(runNumber),
   m_eventNumber(eventNumber),
   m_numberOfEventInThisData(numberOfEventInThisData),
   m_eventTimeStamp(eventTimeStamp),
   m_frameSubSet_intervalle_startTime(0),
   m_hitvec(vec),
   m_DIFtimeInfo(),
   m_originalCollectionParameters(NULL),
   m_buildClusterVec(false)
{}

RawHit_SDHCAL_Data::RawHit_SDHCAL_Data(const std::vector<RawCalorimeterHitPointer>&vec, const RawHit_SDHCAL_Data &d, int numberOfEventInThisData) 
  : m_runNumber(d.getRunNumber()),
    m_eventNumber(d.getEventNumber()),
    m_numberOfEventInThisData(numberOfEventInThisData),
    m_eventTimeStamp(d.getEventTimeStamp()),
    m_frameSubSet_intervalle_startTime(d.m_frameSubSet_intervalle_startTime),
    m_hitvec(vec),
    m_DIFtimeInfo(d.DIFtimeInfo()),
    m_originalCollectionParameters(d.getCollectionParameters()),
    m_buildClusterVec(false)
{}

RawHit_SDHCAL_Data::RawHit_SDHCAL_Data(const RawHitClustersVec &clusters, const RawHit_SDHCAL_Data &d, int numberOfEventInThisData) 
  : m_runNumber(d.getRunNumber()),
    m_eventNumber(d.getEventNumber()),
    m_numberOfEventInThisData(numberOfEventInThisData),
    m_eventTimeStamp(d.getEventTimeStamp()),
    m_frameSubSet_intervalle_startTime(d.m_frameSubSet_intervalle_startTime),
    m_DIFtimeInfo(d.DIFtimeInfo()),
    m_originalCollectionParameters(d.getCollectionParameters()),
    m_buildClusterVec(false)
{
  for (RawHitClustersVec::const_iterator itcluster=clusters.begin(); itcluster!=clusters.end(); ++itcluster)
    {
      RawHitCluster cl;
      for (std::set<const RawCalorimeterHitPointer*>::const_iterator ithit=itcluster->begin(); ithit!=itcluster->end(); ++ithit)
	{
	  m_hitvec.push_back(**ithit);
	  cl.add(*(m_hitvec.rbegin()));
	}
      m_clusters.push_back(cl);
    }
}


void RawHit_SDHCAL_Data::FillTimeInfo(const EVENT::LCParameters& param)
{
  std::vector< std::string > intKeys;
  param.getIntKeys(intKeys);
  for (std::vector< std::string >::iterator itkey=intKeys.begin(); itkey!=intKeys.end(); ++itkey)
    {
      if (param.getNInt(*itkey)!=8) continue; 
      std::vector<int> values;
      param.getIntVals(*itkey,values);
      if (itkey->substr(0,3)!="DIF") continue;
      itkey->erase(0,3);
      if (itkey->substr(itkey->find("_"))!="_Triggers") continue;
      itkey->erase(itkey->find("_"));
      unsigned int dif=atoi(itkey->c_str());
      m_DIFtimeInfo[dif].DTC=values[0];
      m_DIFtimeInfo[dif].GTC=values[1];
      m_DIFtimeInfo[dif].BCID=values[2];
      m_DIFtimeInfo[dif].AbsBCID = values[4];
      m_DIFtimeInfo[dif].AbsBCID = (m_DIFtimeInfo[dif].AbsBCID << 24) + (values[3]&0xFFFFFF);
    }
}

