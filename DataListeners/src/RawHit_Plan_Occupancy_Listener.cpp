#include "RawHit_Plan_Occupancy_Listener.h"

#include "RawCalorimeterHitPointerTools.h"
#include "RawHit_SDHCAL_Data.h"

#include "GG_counter_histos.h"

#include "TDirectory.h"
#include "TH1F.h"

#include <iostream>

RawHit_Plan_Occupancy_Listener::RawHit_Plan_Occupancy_Listener(ExperimentalSetup& setup) : m_setup(setup), m_noiseScale(1), m_unit("")
{
  m_total=0;
  m_DIFnumber_of_the_BIF=m_setup.getBIF();
  std::vector<DIFdrivenDevice*> plans=m_setup.getPlans();
  for (std::vector<DIFdrivenDevice*>::iterator it=plans.begin(); it!= plans.end(); ++it)
    {
      if ((*it)->DIFnumbers().size()>0 && m_setup.DIFnumberIsPad((*it)->DIFnumbers()[0])) 
	{
	  for (unsigned iDIF=0; iDIF<(*it)->DIFnumbers().size(); ++iDIF)
	    m_padDIFs.addValue((*it)->DIFnumbers()[iDIF]);
	}
      if ((*it)->DIFnumbers().size()>0 && m_setup.DIFnumberIsStrip((*it)->DIFnumbers()[0])) 
	{ 
	  m_stripEvenDifs.addValue((*it)->DIFnumbers()[0]); 
	  m_stripOddDifs.addValue((*it)->DIFnumbers()[1]);
	}
    }
}

void RawHit_Plan_Occupancy_Listener::process(const RawHit_SDHCAL_Data& d)
{
  m_total+=d.getNumberOfEventInThisData();
  for (std::vector<RawCalorimeterHitPointer>::const_iterator it=d.getHitVector().begin(); it !=d.getHitVector().end(); ++it)
    {
      if (m_setup.DIFnumberIsBIF(it->dif())) continue;
      unsigned int keys[2]={m_setup.getPlanNumber(it->dif()),it->asic()};
      m_PlaneAsicCounters.add(1,keys);
    }
  m_PlaneAsicCounters.newSet();
}

void RawHit_Plan_Occupancy_Listener::efficiencyReport()
{
 std::cout << "Efficiency report from RawHit_Plan_Occupancy_Listener :"<<std::endl;
 if (m_total==0) {std::cout << "Nothing to report" << std::endl; return;}
 for (std::map<unsigned int,MappedCounters<SingleCounter> >::iterator itplane=m_PlaneAsicCounters.begin(); itplane!=m_PlaneAsicCounters.end(); ++itplane)
    {
      std::cout << "Plane number " << itplane->first << "  :  efficiency=  " << itplane->second.flagcount()/float(m_total);
      if (m_setup.PlanIsStrip(itplane->first)) std::cout << "  strip efficiency # gap 1 : " <<  itplane->second[1].flagcount()/float(m_total) << " gap 2 : " <<  itplane->second[2].flagcount()/float(m_total);
      std::cout << std::endl;
    }
}

void RawHit_Plan_Occupancy_Listener::noiseReport()
{
 std::cout << "Noise report from RawHit_Plan_Occupancy_Listener :"<<std::endl;
 if (m_total==0) {std::cout << "Nothing to report" << std::endl; return;}
 for (std::map<unsigned int,MappedCounters<SingleCounter> >::iterator itplane=m_PlaneAsicCounters.begin(); itplane!=m_PlaneAsicCounters.end(); ++itplane)
    {
      std::cout << "Plane number " << itplane->first << "  :  noise rate=  " << itplane->second.sumcount()/float(m_total)*m_noiseScale << m_unit;
      if (m_setup.PlanIsStrip(itplane->first)) std::cout << "  strip noise rate # gap 1 : " <<  itplane->second[1].sumcount()/float(m_total)*m_noiseScale << m_unit << " gap 2 : " <<  itplane->second[2].sumcount()/float(m_total)*m_noiseScale << m_unit;
      std::cout << std::endl;
    }  
}

void RawHit_Plan_Occupancy_Listener::saveTo(TDirectory* ROOTdir)
{
  if (m_total==0) return;
  if (NULL==ROOTdir) return;
  ROOTdir->cd();
  std::pair<TH1F*,TH1F*> planeHisto=convert(m_PlaneAsicCounters,"Plane_occupancy","Plane",1/float(m_total));
  planeHisto.first->Scale(m_noiseScale);
  planeHisto.first->Write();
  planeHisto.second->Write();

  MappedCounters<SingleCounter> forGapEfficiencies;
  for (std::map<unsigned int,MappedCounters<SingleCounter> >::iterator itplane=m_PlaneAsicCounters.begin(); itplane!=m_PlaneAsicCounters.end(); ++itplane)
    if (m_setup.PlanIsStrip(itplane->first)) 
      {
	forGapEfficiencies[10*itplane->first]=m_PlaneAsicCounters[itplane->first];
	forGapEfficiencies[1+10*itplane->first]=m_PlaneAsicCounters[itplane->first][1];
	forGapEfficiencies[2+10*itplane->first]=m_PlaneAsicCounters[itplane->first][2];
      }
  std::pair<TH1F*,TH1F*> gapHisto=convert(forGapEfficiencies,"Gap_occupancy","Strip plane or gap",1/float(m_total));
  gapHisto.first->Scale(m_noiseScale);
  gapHisto.first->Write();
  gapHisto.second->Write();
}
