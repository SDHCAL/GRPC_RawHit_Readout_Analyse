#include "RawHit_SDHCAL_Data_LCWriter_CalorimeterHit.h"

#include "domain.h"
#include "RawHit_SDHCAL_Data.h"
#include "RawCalorimeterHitPointerTools.h"
#include "IMPL/LCCollectionVec.h"
#include "IMPL/LCFlagImpl.h"
#include "IMPL/CalorimeterHitImpl.h"
#include "EVENT/LCIO.h"

#include <sstream>

RawHit_SDHCAL_Data_LCWriter_CalorimeterHit::RawHit_SDHCAL_Data_LCWriter_CalorimeterHit(ExperimentalSetup& setup,std::string collectionName) : RawHit_SDHCAL_Data_LCWriter(collectionName), m_setup(&setup), m_tricot_DIFs(m_setup->getTricotDevice_DIFnumber())
{
  if (m_setup->hasBIF()) m_parametersFromSetup["BIF"]=std::vector<int>(1,m_setup->getBIF());
  std::vector<DIFdrivenDevice*> plans=m_setup->getPlans();
  for (unsigned int i=0; i<plans.size(); ++i)
    {
      std::stringstream ss;
      ss << "Plan_" << i << "_";
      std::vector<unsigned int> uDIFs=plans[i]->DIFnumbers();
      std::vector<int> DIFs;
      for (std::vector<unsigned int>::iterator itdif=uDIFs.begin(); itdif != uDIFs.end(); ++itdif) DIFs.push_back(*itdif);
      if (m_setup->DIFnumberIsStrip(DIFs[0])) ss << "strip";
      else if (m_setup->DIFnumberIsPad(DIFs[0])) ss << "pad";
      else if (m_setup->DIFnumberIsTricot(DIFs[0])) ss << "tricot";
      else  ss << "NotFullyImplementedType";
      m_parametersFromSetup[ss.str()]=DIFs;
    }
}


IMPL::LCCollectionVec* RawHit_SDHCAL_Data_LCWriter_CalorimeterHit::createAndFillCollection(const RawHit_SDHCAL_Data& d)
{
  IMPL::LCCollectionVec *col=new IMPL::LCCollectionVec(EVENT::LCIO::CALORIMETERHIT);
  //Prepare a flag to tag data type in  col (precise ce qu'on va enregistrer)
  IMPL::LCFlagImpl chFlag(0) ;
  chFlag.setBit(EVENT::LCIO::RCHBIT_LONG ) ;                  // To be set when position will be provided
  chFlag.setBit(EVENT::LCIO::RCHBIT_BARREL ) ;                  // barrel
  chFlag.setBit(EVENT::LCIO::RCHBIT_ID1 ) ;                     // cell ID1 set (ID0 du raw calorimeter hit )
  chFlag.setBit(EVENT::LCIO::RCHBIT_TIME ) ;                    // timestamp set
  col->setFlag( chFlag.getFlag() ) ; 

  float pos[3];
  EVENT::IntVec BIFvaluesFound;

  
  if (! m_tricot_DIFs.empty())
    {
      std::vector<RawCalorimeterHitPointer> tricot_RawHits=extract(d.getHitVector(),domain<unsigned int>(m_tricot_DIFs),rawHit_DIF());
      for (auto DIFnum : m_tricot_DIFs)
	{
	  std::vector<RawCalorimeterHitPointer> OneDIF_RawHit=extract(tricot_RawHits,domain<unsigned int>(DIFnum),rawHit_DIF());
	  m_setup->getTricotDevice(DIFnum)->computeHitPosition(OneDIF_RawHit);
	}
    }
  
  for (std::vector<RawCalorimeterHitPointer>::const_iterator itHit=d.getHitVector().begin(); itHit!=d.getHitVector().end(); ++itHit)
    {
      if (m_setup->DIFnumberIsBIF(itHit->dif()))
	{
	  BIFvaluesFound.push_back((*itHit)->getAmplitude());
	  continue;
	}
      unsigned int I,J,K;
      m_setup->getCoord3D(itHit->dif(), itHit->asic(), itHit->channel(), I, J, K);
      unsigned int ID0=((K&0xFF)<<16)+((J&0xFF)<<8)+(I&0xFF);
      
      IMPL::CalorimeterHitImpl *newHit=new IMPL::CalorimeterHitImpl();
      newHit->setCellID0(ID0);
      newHit->setCellID1((*itHit)->getCellID0());
      newHit->setEnergy((*itHit)->getAmplitude());
      newHit->setTime((*itHit)->getTimeStamp());
      m_setup->getOrAddDevice(itHit->dif()).getAbsolutePositionIn_mm(I,J,pos); //could try to optimise this
      newHit->setPosition(pos)  ; //FIXME : Write a WARNING when not implemented
      col->addElement(newHit);
    }
  col->parameters().setValues("BIF_amplitudes",BIFvaluesFound);
  return col;
}

void RawHit_SDHCAL_Data_LCWriter_CalorimeterHit::finalizeCollection(IMPL::LCCollectionVec* col)
{
  //Fixme, should recopy value if it exists
  col->parameters().setValue( "CellID1Encoding", "DIF:8,ASIC:8,Channel:6" );

  col->parameters().setValue( EVENT::LCIO::CellIDEncoding, "I:8,J:8,K:8,DIF:32:8,ASIC:8,Channel:6" );
  
  for (std::map<std::string, std::vector<int> >::iterator it=m_parametersFromSetup.begin(); it != m_parametersFromSetup.end(); ++it)
    col->parameters().setValues(it->first,it->second);
    
}
