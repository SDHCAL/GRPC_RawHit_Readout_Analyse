#include "RawHit_Plan_Cluster_Occupancy_Listener.h"

#include "RawCalorimeterHitClustering.h"
#include "RawHit_SDHCAL_Data.h"
#include "RawCalorimeterHitPointerExperimentalSetupTools.h"

#include "GG_counter_histos.h"

#include "TDirectory.h"
#include "TH1F.h"

#include <iostream>
#include <iterator>

RawHit_Plan_Cluster_Occupancy_Listener::RawHit_Plan_Cluster_Occupancy_Listener(ExperimentalSetup& setup) : m_setup(setup)
{}

void RawHit_Plan_Cluster_Occupancy_Listener::process(const RawHit_SDHCAL_Data& d)
{
  if (not d.hasCluster()) return;
  for (unsigned int icluster=0; icluster<d.getClusterBounds().size()-1; ++icluster)
    {
      unsigned int keys[3];
      keys[0]=mostFrequentValue(d.getClusterBounds()[icluster],d.getClusterBounds()[icluster+1],rawHit_PlanNumber(m_setup));
      if (keys[0]==BaseExperimentalSetupGetter::ERRORCODE) continue;
      keys[1]=int(mean(d.getClusterBounds()[icluster],d.getClusterBounds()[icluster+1],rawHit_JorGap(m_setup)));
      keys[2]=int(mean(d.getClusterBounds()[icluster],d.getClusterBounds()[icluster+1],rawHit_IorStrip(m_setup)));
      //for each keys[X], the BaseExperimentalSetupGetter computes I,J,K for each hit (3 times the same computation, not very efficient)      
      m_setupCounter.add(std::distance(d.getClusterBounds()[icluster],d.getClusterBounds()[icluster+1]),keys);
    }
}


void RawHit_Plan_Cluster_Occupancy_Listener::saveTo(TDirectory* ROOTdir)
{
  //std::string labels[4]={"Setup","Plan","JorGap","IorStrip"}; m_setupCounter.write(labels);
  if (m_setupCounter.empty()) return;
  if (NULL==ROOTdir) return;
  ROOTdir->cd();
  OneLevelMappedSingleMapCounterHistos histos=convert(m_setupCounter,"Cluster","Cluster size","Plane number","Cluster size");
  histos.UpperLevelDistribution->Write();
  for (unsigned int i=0; i<m_setupCounter.size(); ++i)
    histos.BottomLevelDistribution[i]->Write();
  histos.BottomLevelDistributionProfile->Write();

  TProfile2D** prof2D=convert2D(m_setupCounter,"Cluster_2D", "Cluster size","Plane number", "I or strip", "J or gap");
  for (unsigned int i=0; i<m_setupCounter.size(); ++i)
    prof2D[i]->Write();
}
