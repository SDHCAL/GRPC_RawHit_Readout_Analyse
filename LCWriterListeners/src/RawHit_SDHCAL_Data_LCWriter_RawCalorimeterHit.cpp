#include "RawHit_SDHCAL_Data_LCWriter_RawCalorimeterHit.h"

#include "RawHit_SDHCAL_Data.h"
#include "IMPL/LCCollectionVec.h"
#include "IMPL/LCFlagImpl.h"
#include "IMPL/RawCalorimeterHitImpl.h"
#include "EVENT/LCIO.h"

IMPL::LCCollectionVec* RawHit_SDHCAL_Data_LCWriter_RawCalorimeterHit::createAndFillCollection(const RawHit_SDHCAL_Data& d)
{
  IMPL::LCCollectionVec *col=new IMPL::LCCollectionVec(EVENT::LCIO::RAWCALORIMETERHIT);
  //Prepare a flag to tag data type in  col (precise ce qu'on va enregistrer)
  IMPL::LCFlagImpl chFlag(0) ;
  chFlag.setBit(EVENT::LCIO::RCHBIT_BARREL ) ;                  // barrel
  chFlag.setBit(EVENT::LCIO::RCHBIT_ID1 ) ;                     // cell ID1 set (c'est le FrameBCID )
  chFlag.setBit(EVENT::LCIO::RCHBIT_TIME ) ;                    // timestamp set
  col->setFlag( chFlag.getFlag() ) ; 

  for (std::vector<RawCalorimeterHitPointer>::const_iterator itHit=d.getHitVector().begin(); itHit!=d.getHitVector().end(); ++itHit)
    {
      IMPL::RawCalorimeterHitImpl *newHit=new IMPL::RawCalorimeterHitImpl();
      newHit->setCellID0((*itHit)->getCellID0());
      newHit->setCellID1((*itHit)->getCellID1());
      newHit->setAmplitude((*itHit)->getAmplitude());
      newHit->setTimeStamp((*itHit)->getTimeStamp());
      col->addElement(newHit);
    }
  return col;
}

void RawHit_SDHCAL_Data_LCWriter_RawCalorimeterHit::finalizeCollection(IMPL::LCCollectionVec* col)
{
  col->parameters().setValue( EVENT::LCIO::CellIDEncoding, "DIF:8,ASIC:8,Channel:6" );
}
