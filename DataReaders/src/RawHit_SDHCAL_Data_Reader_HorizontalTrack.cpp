#include "RawHit_SDHCAL_Data_Reader_HorizontalTrack.h"

#include "RawHit_SDHCAL_Data.h"
#include "Clustering.h"
#include "RawHit_horizontal_track_predicate.h"
#include "RawCalorimeterHitClustering.h"
#include "RawCalorimeterHitPointerTools.h"
#include "intervalle.h"
#include <set>
#include <iostream>

RawHit_SDHCAL_Data_Reader_HorizontalTrack::~RawHit_SDHCAL_Data_Reader_HorizontalTrack()
{
  std::cout << "RawHit_SDHCAL_Data_Reader_HorizontalTrack " << m_trackEvents << " track event seen." << std::endl; 
}

void RawHit_SDHCAL_Data_Reader_HorizontalTrack::process(const RawHit_SDHCAL_Data& d)
{
  std::vector<RawCalorimeterHitPointer> toCluster=d.getHitVector();
  std::vector<std::vector<RawCalorimeterHitPointer>::iterator> clusterBounds;
  SDHCAL::clusterize(toCluster.begin(),toCluster.end(),clusterBounds,RawHit_horizontal_track_predicate(m_setup));
  unsigned int ntracks=0;
  for (unsigned int i=0; i<clusterBounds.size()-1; ++i)
    if (clusterIsOK(clusterBounds[i],clusterBounds[i+1]))
      {
	std::map<unsigned int, unsigned int> m=distribution(clusterBounds[i],clusterBounds[i+1],rawHit_TimeStamp());
	UI_intervalle clusterTimeSpan(m.begin()->first,m.rbegin()->first);
	if (clusterTimeSpan.length()>m_maxClusterSpan) continue;
	if (clusterTimeSpan.first > m_expandClusterTime)
	  clusterTimeSpan.first -= m_expandClusterTime;
	else
	  clusterTimeSpan.first=0;
	clusterTimeSpan.second+=m_expandClusterTime;
	std::vector<RawCalorimeterHitPointer> trackEvent=extract(toCluster,clusterTimeSpan,rawHit_TimeStamp());
	addBIFhit(toCluster,trackEvent,clusterTimeSpan);
	RawHit_SDHCAL_Data newd(trackEvent,d);
	newd.setFrameSubSet_intervalle_startTime(clusterTimeSpan.first);
	newd.clusterize(RawHit_horizontal_track_predicate(m_setup));
	notifyListeners(newd);
	++ntracks;
      }
  if (m_verbose) std::cout << ++m_nevent << " : " << toCluster.size() << " hits clustered in " << clusterBounds.size()-1 << " clusters for " << ntracks << " tracks.\r"; std::cout.flush();
  m_trackEvents+=ntracks;
}

bool RawHit_SDHCAL_Data_Reader_HorizontalTrack::clusterIsOK(std::vector<RawCalorimeterHitPointer>::iterator &begin,std::vector<RawCalorimeterHitPointer>::iterator &end)
{
  std::set<unsigned int> planNumberOfCluster;
  for (std::vector<RawCalorimeterHitPointer>::iterator it=begin; it != end; ++it)
    if (not m_setup.DIFnumberIsBIF(it->dif()))
      planNumberOfCluster.insert(m_setup.getPlanNumber(it->dif()));
  return planNumberOfCluster.size()>=m_minNumberOfPlan;
}

void RawHit_SDHCAL_Data_Reader_HorizontalTrack::addBIFhit(std::vector<RawCalorimeterHitPointer>& event,std::vector<RawCalorimeterHitPointer>& trackEvent,UI_intervalle &timeSpan)
{
  if (not m_setup.hasBIF()) return;
  int maxBIFtime=timeSpan.second+m_BIFtimeWindow.second;
  if (maxBIFtime<0) return;
  int minBIFtime=timeSpan.first+m_BIFtimeWindow.first;
  if (minBIFtime<0) minBIFtime=0;
  UI_intervalle BIFtimeWindow(minBIFtime,maxBIFtime);
  std::vector<RawCalorimeterHitPointer> BIF_hitvector=extract(event,m_setup.getBIF(),rawHit_DIF());
  BIF_hitvector=extract(BIF_hitvector,BIFtimeWindow,rawHit_TimeStamp());
  trackEvent.insert(trackEvent.end(),BIF_hitvector.begin(),BIF_hitvector.end());
}
