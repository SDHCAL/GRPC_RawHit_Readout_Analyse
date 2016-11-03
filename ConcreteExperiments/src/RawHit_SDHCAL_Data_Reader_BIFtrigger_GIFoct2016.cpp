#include "RawHit_SDHCAL_Data_Reader_BIFtrigger_GIFoct2016.h"

#include "RawCalorimeterHitPointerTools.h"
#include "RawHit_SDHCAL_Data.h"

#include "GG_counter_histos.h"

#include "TDirectory.h"
#include "TH1F.h"

#include <iostream>

RawHit_SDHCAL_Data_Reader_BIFtrigger_GIFoct2016::RawHit_SDHCAL_Data_Reader_BIFtrigger_GIFoct2016()
{
  m_DIFnumber_of_the_BIF=m_setup.getBIF();
  std::vector<DIFdrivenDevice*> plans=m_setup.getPlans();
  for (std::vector<DIFdrivenDevice*>::iterator it=plans.begin(); it!= plans.end(); ++it)
    {
      if ((*it)->DIFnumbers().size()==1) m_padDIFs.addValue((*it)->DIFnumbers()[0]);
      if ((*it)->DIFnumbers().size()==2) { m_stripEvenDifs.addValue((*it)->DIFnumbers()[0]); m_stripOddDifs.addValue((*it)->DIFnumbers()[1]);}
    }
}

void RawHit_SDHCAL_Data_Reader_BIFtrigger_GIFoct2016::process(const RawHit_SDHCAL_Data& d)
{
  RawHit_SDHCAL_Data dBIF(std::vector<RawCalorimeterHitPointer>(),d);

  std::vector<RawCalorimeterHitPointer> padData=extract(d.getHitVector(),m_padDIFs,rawHit_DIF());
  padData=extract(padData,UI_intervalle(15,30),rawHit_Ilocal());
  padData=extract(padData,UI_intervalle(15,18),rawHit_Jlocal());

  std::vector<RawCalorimeterHitPointer> stripEvenData=extract(d.getHitVector(),m_stripEvenDifs,rawHit_DIF());
  stripEvenData=extract(stripEvenData,UI_intervalle(48,75),rawHit_EvenStrip());
  padData.insert(padData.end(),stripEvenData.begin(),stripEvenData.end());

  std::vector<RawCalorimeterHitPointer> stripOddData=extract(d.getHitVector(),m_stripOddDifs,rawHit_DIF());
  stripOddData=extract(stripOddData,UI_intervalle(48,75),rawHit_OddStrip());
  padData.insert(padData.end(),stripOddData.begin(),stripOddData.end());

  std::vector<RawCalorimeterHitPointer> BIF_hitvector=extract(d.getHitVector(),m_DIFnumber_of_the_BIF,rawHit_DIF());
  padData.insert(padData.end(),BIF_hitvector.begin(),BIF_hitvector.end());

  dBIF.replaceVec(padData);
  notifyListeners(dBIF);

  m_total++;
  for (std::vector<RawCalorimeterHitPointer>::const_iterator it=dBIF.getHitVector().begin(); it !=dBIF.getHitVector().end(); ++it)
    {
      if (m_setup.DIFnumberIsBIF(it->dif())) continue;
      unsigned int keys[2]={m_setup.getPlanNumber(it->dif()),it->asic()};
      m_PlaneAsicCounters.add(1,keys);
    }
  m_PlaneAsicCounters.newSet();

}

void RawHit_SDHCAL_Data_Reader_BIFtrigger_GIFoct2016::reportAndSaveTo(TDirectory* ROOTdir)
{
  std::cout << "Report from RawHit_SDHCAL_DataReader_BIFtrigger_GIFoct2016 :"<<std::endl;
  if (m_total==0) return;
  for (std::map<unsigned int,MappedCounters<SingleCounter> >::iterator itplane=m_PlaneAsicCounters.begin(); itplane!=m_PlaneAsicCounters.end(); ++itplane)
    {
      std::cout << "Plane number " << itplane->first << "  :  efficiency=  " << itplane->second.flagcount()/float(m_total);
      if (m_setup.PlanIsStrip(itplane->first)) std::cout << "  strip efficiency # gap 1 : " <<  itplane->second[1].flagcount()/float(m_total) << " gap 2 : " <<  itplane->second[2].flagcount()/float(m_total);
      std::cout << std::endl;
    }

  if (NULL==ROOTdir) return;
  ROOTdir->cd();
  std::pair<TH1F*,TH1F*> planeHisto=convert(m_PlaneAsicCounters,"Plane_occupancy","Plane",1/float(m_total));
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
  gapHisto.first->Write();
  gapHisto.second->Write();
  
}
