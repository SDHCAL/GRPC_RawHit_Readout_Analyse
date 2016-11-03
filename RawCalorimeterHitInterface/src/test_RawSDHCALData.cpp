// test program

#include <assert.h>

#include "IMPL/LCCollectionVec.h"

#include "IMPL/LCFlagImpl.h"
#include "EVENT/LCIO.h"
#include "IMPL/LCParametersImpl.h"

#include "RawHit_SDHCAL_Data.h"
#include "RawCalorimeterHitUtilities.h"

#include <map>
#include <string>

IMPL::LCCollectionVec* createFakeRawHitCollection(unsigned int nHit)
{
  std::map<unsigned int,std::string> keyTriggers;
  keyTriggers[3]="DIF3_Triggers";
  keyTriggers[4]="DIF4_Triggers";
  keyTriggers[5]="DIF5_Triggers";

  IMPL::LCCollectionVec *col=new IMPL::LCCollectionVec(EVENT::LCIO::RAWCALORIMETERHIT);
  IMPL::LCFlagImpl chFlag(0) ;
  chFlag.setBit(EVENT::LCIO::RCHBIT_TIME ) ;                    // timestamp set
  col->setFlag( chFlag.getFlag() ) ; 
  for (unsigned int i=0; i<nHit; ++i)
    col->addElement(createRawHit(1+i%3,4*i,3+i%3,1+i%12, i%64));
  std::vector<int> values(8);
  values[0]=31;  //DTC
  values[1]=10;  //GTC
  values[2]=133; //BCID
  values[3]=444; //AbsBCID lower bits
  values[4]=1;   //AbsBCID upper bits
  values[5]=0;   //TDIF : unused
  values[6]=0;   //TASU1 : unused
  values[7]=0;   //TASU2 : unused

  col->parameters().setValues(keyTriggers[3],values);
  values[0]=41;
  col->parameters().setValues(keyTriggers[4],values);
  values[0]=51;
  col->parameters().setValues(keyTriggers[5],values);

  return col;
}

void testDataContent(const RawHit_SDHCAL_Data& d)
{
  assert( d.getRunNumber()==16 );
  assert( d.getEventNumber()==2 );
  assert( d.getEventTimeStamp()==720 );
  assert( d.DIFtimeInfo().size()==3 );
  assert( d.getCollectionParameters()!=NULL ); 
  
  std::map<unsigned int,DIF_timeInfo>::iterator it=d.DIFtimeInfo().begin();
  assert( it->first==3 );
  ++it; assert( it->first==4 );
  ++it; assert( it->first==5 );

  for (it=d.DIFtimeInfo().begin(); it!=d.DIFtimeInfo().end(); ++it)
    {
      assert(it->second.DTC==10*it->first+1);
      assert(it->second.GTC==10);
      assert(it->second.BCID==133);
      assert( (it->second.AbsBCID&0xFFFFFFFF)==444 );
      assert( (it->second.AbsBCID>>32)==1 );
    }
}


int main()
{
  std::vector<RawCalorimeterHitPointer> myvec;
  RawHit_SDHCAL_Data a(myvec, 5, 6, 144);
  assert( a.getRunNumber()==5 );
  assert( a.getEventNumber()==6 );
  assert( a.getEventTimeStamp()==144 );
  assert( a.getHitVector().empty() );
  assert( a.DIFtimeInfo().empty() );
  assert( a.getCollectionParameters()==NULL ); 

  std::vector<RawCalorimeterHitPointer> myvecBis;
  myvecBis.push_back(createRawHit(1,1,1,1, 1));
  a.replaceVec(myvecBis);
  assert( a.getHitVector().size()==1 );
  assert( a.DIFtimeInfo().empty() );

  IMPL::LCParametersImpl LCparam;
  LCparam.setValue("DUMMY",35);
  a.FillTimeInfo(LCparam);
  assert( a.getCollectionParameters()==NULL ); 
  assert( a.DIFtimeInfo().empty() );

  std::vector<int> info(8,4);
  LCparam.setValues("DIF44_Triggers",info);
  a.FillTimeInfo(LCparam);
  assert( a.getCollectionParameters()==NULL ); 
  assert( a.DIFtimeInfo().size()==1 );
  assert( a.DIFtimeInfo().begin()->first==44 );
  DIF_timeInfo di=a.DIFtimeInfo().begin()->second;
  assert(di.DTC==4);
  assert(di.GTC==4);
  assert(di.BCID==4);
  assert( (di.AbsBCID&0xFFFFFFFF)==4 );
  assert( (di.AbsBCID>>32)==4 );

  

  IMPL::LCCollectionVec* col=createFakeRawHitCollection(12);
  RawHit_SDHCAL_Data b(*col, 16, 2, 720);
  testDataContent(b);
  assert( b.getHitVector().size()==12 );
  for (unsigned int i=0; i<b.getHitVector().size(); ++i)
    {
      assert(b.getHitVector()[i].dif()==3+i%3);
      assert(b.getHitVector()[i]->getTimeStamp()==4*i);
    }

  RawHit_SDHCAL_Data c(myvecBis,b);
  testDataContent(c);
  assert( c.getHitVector().size()==1 );


  return 0;
}
