#include "RawHit_SDHCAL_Data_Reader_BIFtrigger_GIFoct2016.h"

#include "RawCalorimeterHitPointerTools.h"
#include "RawHit_SDHCAL_Data.h"

#include "TDirectory.h"
#include "TH1F.h"

#include <iostream>

RawHit_SDHCAL_Data_Reader_BIFtrigger_GIFoct2016::RawHit_SDHCAL_Data_Reader_BIFtrigger_GIFoct2016()
{
  m_DIFnumber_of_the_BIF=m_setup.getBIF(0); //this setup has only one BIF
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
}

