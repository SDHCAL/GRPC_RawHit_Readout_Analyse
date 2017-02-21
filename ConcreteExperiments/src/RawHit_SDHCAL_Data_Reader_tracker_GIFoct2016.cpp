#include "RawHit_SDHCAL_Data_Reader_tracker_GIFoct2016.h"

#include "RawCalorimeterHitPointerTools.h"
#include "RawCalorimeterHitClustering.h"
#include "RawHit_SDHCAL_Data.h"

#include "TDirectory.h"
#include "TH1F.h"

#include <iostream>
#include <set>
#include <map>

RawHit_SDHCAL_Data_Reader_tracker_GIFoct2016::RawHit_SDHCAL_Data_Reader_tracker_GIFoct2016()
{
  m_DIFnumber_of_the_BIF=m_setup.getBIF();
  std::vector<DIFdrivenDevice*> plans=m_setup.getPlans();
  for (std::vector<DIFdrivenDevice*>::iterator it=plans.begin(); it!= plans.end(); ++it)
    {
      if ((*it)->DIFnumbers().size()==1) m_padDIFs.addValue((*it)->DIFnumbers()[0]);
      if ((*it)->DIFnumbers().size()==2) { m_stripEvenDifs.addValue((*it)->DIFnumbers()[0]); m_stripOddDifs.addValue((*it)->DIFnumbers()[1]);}
    }
}

void RawHit_SDHCAL_Data_Reader_tracker_GIFoct2016::process(const RawHit_SDHCAL_Data& d)
{
  RawHit_SDHCAL_Data dBIF(std::vector<RawCalorimeterHitPointer>(),d);

  //search the track:
  UI_intervalle Itrack(0,9999),Jtrack;
  for (unsigned int i=0; i<d.getClusters().size()-1; ++i)
    {
      std::set<unsigned int> planCounting;
      std::map<unsigned int, unsigned int> difInCluster=distribution(d.getClusterBounds()[i],d.getClusterBounds()[i+1],rawHit_DIF());
      for (std::map<unsigned int, unsigned int>::iterator itmap=difInCluster.begin(); itmap != difInCluster.end(); ++itmap)
	if (m_padDIFs.valueInDomain(itmap->first))
	  planCounting.insert(itmap->first);
      if (planCounting.size()>=3)
	{
	  std::map<unsigned int, unsigned int> Idistr=distribution(d.getClusterBounds()[i],d.getClusterBounds()[i+1],rawHit_Ilocal());
	  Itrack=UI_intervalle(Idistr.begin()->first,Idistr.rbegin()->first);
	  std::map<unsigned int, unsigned int> Jdistr=distribution(d.getClusterBounds()[i],d.getClusterBounds()[i+1],rawHit_Jlocal());
	  Jtrack=UI_intervalle(Jdistr.begin()->first,Jdistr.rbegin()->first);
	  break;
	}
    }

  if (Itrack.second==9999) return;
  
  std::vector<RawCalorimeterHitPointer> padData=extract(d.getHitVector(),m_padDIFs,rawHit_DIF());
  padData=extract(padData,Itrack,rawHit_Ilocal());
  padData=extract(padData,Jtrack,rawHit_Jlocal());

  float matchfactor=11.0/3;
  UI_intervalle stripIntervalle((unsigned int) floor(Jtrack.first*matchfactor),(unsigned int) ceil(Jtrack.second*matchfactor));
  
  std::vector<RawCalorimeterHitPointer> stripEvenData=extract(d.getHitVector(),m_stripEvenDifs,rawHit_DIF());
  stripEvenData=extract(stripEvenData,stripIntervalle,rawHit_EvenStrip());
  padData.insert(padData.end(),stripEvenData.begin(),stripEvenData.end());

  std::vector<RawCalorimeterHitPointer> stripOddData=extract(d.getHitVector(),m_stripOddDifs,rawHit_DIF());
  stripOddData=extract(stripOddData,stripIntervalle,rawHit_OddStrip());
  padData.insert(padData.end(),stripOddData.begin(),stripOddData.end());

  std::vector<RawCalorimeterHitPointer> BIF_hitvector=extract(d.getHitVector(),m_DIFnumber_of_the_BIF,rawHit_DIF());
  padData.insert(padData.end(),BIF_hitvector.begin(),BIF_hitvector.end());

  dBIF.replaceVec(padData);
  notifyListeners(dBIF);
}

