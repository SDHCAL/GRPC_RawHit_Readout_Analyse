// test program

#include <assert.h>

#include "intervalle.h"
#include "domain.h"

#include "RawCalorimeterHitPointer.h"
#include "RawCalorimeterHitPointerTools.h"
#include "RawCalorimeterHitUtilities.h"


#include <algorithm>

int main()
{

  std::vector<RawCalorimeterHitPointer> hitvec;
// int                          amplitude, timestamp, dif, asic, channel  
  hitvec.push_back(createRawHit(        1,         5,   3,    2,      13));
  hitvec.push_back(createRawHit(        2,         6,   3,    1,      14));
  hitvec.push_back(createRawHit(        3,        12,   1,   33,      15));
  hitvec.push_back(createRawHit(        1,         7,  54,   12,      16));
  hitvec.push_back(createRawHit(        1,         8,   3,    1,      17));
  hitvec.push_back(createRawHit(        6,       108,   7,   33,      13));


  assert(hitvec[0].dif()==3);
  assert(hitvec[0].asic()==2);
  assert(hitvec[0].channel()==13);
  assert(hitvec[0].thresholdCrossed()==1);
  assert(hitvec[0]->getAmplitude()==1);
  assert(hitvec[0]->getTimeStamp()==5);
  assert(hitvec[0].amplitudeInfo()==0);
  assert(hitvec[5].thresholdCrossed()==2);
  assert(hitvec[5]->getAmplitude()==6);
  assert(hitvec[5].amplitudeInfo()==1);


  //test RawCalorimeterHitPointer tools
  print(hitvec, std::cout);

  std::vector<RawCalorimeterHitPointer> testedVec;
  std::cout<<"time stamp inside 7,12" << std::endl; 
  testedVec=extract(hitvec,UI_domain(UI_intervalle(7,12)),rawHit_TimeStamp()); print( testedVec, std::cout);
  assert(testedVec.size()==3); for (std::vector<RawCalorimeterHitPointer>::iterator it=testedVec.begin(); it!=testedVec.end(); ++it) assert((*it)->getTimeStamp()>=7 && (*it)->getTimeStamp()<=12);
  testedVec=extract(hitvec,UI_intervalle(7,12),rawHit_TimeStamp()); print( testedVec, std::cout);
  assert(testedVec.size()==3); for (std::vector<RawCalorimeterHitPointer>::iterator it=testedVec.begin(); it!=testedVec.end(); ++it) assert((*it)->getTimeStamp()>=7 && (*it)->getTimeStamp()<=12);

  std::cout<<"time stamp outside 7,12" << std::endl;
  testedVec=extract(hitvec,UI_domain(UI_intervalle(7,12)),rawHit_TimeStamp(),false); print( testedVec, std::cout);
  assert(testedVec.size()==3); for (std::vector<RawCalorimeterHitPointer>::iterator it=testedVec.begin(); it!=testedVec.end(); ++it) assert((*it)->getTimeStamp()<7 || (*it)->getTimeStamp()>12);

  std::cout<<"time stamp is 8" << std::endl;
  testedVec=extract(hitvec,8,rawHit_TimeStamp());  print( testedVec, std::cout);
  assert(testedVec.size()==1); assert(testedVec[0]->getTimeStamp()==8);


  std::cout<<"un autre filtre time stamp [3,5] U [7,8]" << std::endl;
  UI_domain Tdomain(UI_intervalle(3,5)); Tdomain.addIntervalle(UI_intervalle(7,8));
  testedVec=extract(hitvec,Tdomain,rawHit_TimeStamp()); print( testedVec, std::cout);
  assert(testedVec.size()==3); for (std::vector<RawCalorimeterHitPointer>::iterator it=testedVec.begin(); it!=testedVec.end(); ++it) assert((*it)->getTimeStamp()>=3 && (*it)->getTimeStamp()<=8 && (*it)->getTimeStamp()!=6);

  std::cout<<"reverse filter time stamp [3,5] U [7,8]" << std::endl;
  testedVec=extract(hitvec,Tdomain,rawHit_TimeStamp(),false); print( testedVec, std::cout);
  assert(testedVec.size()==3); for (std::vector<RawCalorimeterHitPointer>::iterator it=testedVec.begin(); it!=testedVec.end(); ++it) assert((*it)->getTimeStamp()<3 || (*it)->getTimeStamp()>8 || (*it)->getTimeStamp()==6);
 

  std::cout << "hit above first Threshold" <<std::endl; 
  testedVec=extract(hitvec,aboveFirstThreshold(),rawHit_Threshold());
  assert(testedVec.size()==6); print( testedVec, std::cout);
  std::cout << "hit above second Threshold" <<std::endl; 
  testedVec=extract(hitvec,aboveSecondThreshold(),rawHit_Threshold()); print( testedVec, std::cout);
  assert(testedVec.size()==4); for (std::vector<RawCalorimeterHitPointer>::iterator it=testedVec.begin(); it!=testedVec.end(); ++it) assert(it->thresholdCrossed()==1 || it->thresholdCrossed()==3);
  std::cout << "hit above third Threshold" <<std::endl; 
  testedVec=extract(hitvec,aboveThirdThreshold(),rawHit_Threshold()); print( testedVec, std::cout);
  assert(testedVec.size()==1); assert(testedVec[0].thresholdCrossed()==3);
  std::cout << "hit below third Threshold" <<std::endl; 
  testedVec=extract(hitvec,aboveThirdThreshold(),rawHit_Threshold(),false); print( testedVec, std::cout);
  assert(testedVec.size()==5); for (std::vector<RawCalorimeterHitPointer>::iterator it=testedVec.begin(); it!=testedVec.end(); ++it) assert(it->thresholdCrossed()==1 || it->thresholdCrossed()==2);
  
  
  std::cout << "hit with amplitude above 4" << std::endl;
  testedVec=extract(hitvec,UI_intervalle(0,4),rawHit_Amplitude(),false); print( testedVec, std::cout);
  assert(testedVec.size()==1); assert(testedVec[0]->getAmplitude()>4);


  std::cout << "hit in DIF 54" << std::endl;
  testedVec=extract(hitvec,54,rawHit_DIF()); print( testedVec, std::cout);
  assert(testedVec.size()==1); assert(testedVec[0].dif()==54);
  std::cout << "hit not in DIF 54" << std::endl;
  testedVec=extract(hitvec,54,rawHit_DIF(),false); print( testedVec, std::cout);
  assert(testedVec.size()==5); for (std::vector<RawCalorimeterHitPointer>::iterator it=testedVec.begin(); it!=testedVec.end(); ++it) assert(it->dif() != 54);
  std::cout << "hit in ASIC 1" << std::endl;
  testedVec=extract(hitvec,1,rawHit_ASIC()); print( testedVec, std::cout);
  assert(testedVec.size()==2); for (std::vector<RawCalorimeterHitPointer>::iterator it=testedVec.begin(); it!=testedVec.end(); ++it) assert(it->asic() == 1);
  std::cout << "hit in CHANNEL 13" << std::endl;
  testedVec=extract(hitvec,13,rawHit_Channel()); print( testedVec, std::cout);
  assert(testedVec.size()==2); for (std::vector<RawCalorimeterHitPointer>::iterator it=testedVec.begin(); it!=testedVec.end(); ++it) assert(it->channel() == 13);
  std::cout << "hit in ASIC 12 to 63" << std::endl;
  testedVec=extract(hitvec,UI_intervalle(12,63),rawHit_ASIC()); print( testedVec, std::cout);
  assert(testedVec.size()==3); for (std::vector<RawCalorimeterHitPointer>::iterator it=testedVec.begin(); it!=testedVec.end(); ++it) assert(it->asic() >= 12 && it->asic()<= 63);
  
  std::cout << "hit in DIF 3 and above threshold 2" << std::endl;
  std::vector<RawCalorimeterHitPointer> hitvec_channel=extract(hitvec,3,rawHit_DIF());
  testedVec=extract(  hitvec_channel, aboveSecondThreshold() , rawHit_Threshold()); print( testedVec, std::cout);
  assert(testedVec.size()==2); for (std::vector<RawCalorimeterHitPointer>::iterator it=testedVec.begin(); it!=testedVec.end(); ++it) assert(it->dif() ==3 && (it->thresholdCrossed()==1 || it->thresholdCrossed()==3) );


  std::cout << "check for counting " << std::endl;
  assert(std::count_if(hitvec.begin(),hitvec.end(), rawHit_IsEqual<rawHit_Amplitude>(1) )==3);
  assert(std::count_if(hitvec.begin(),hitvec.end(), rawHit_IsEqual<rawHit_ASIC>(1) )==2);
  assert(std::count_if(hitvec.begin(),hitvec.end(), rawHit_IsInIntervalle<rawHit_Amplitude>(1,2) )==4);
  assert(std::count_if(hitvec.begin(),hitvec.end(), rawHit_IsInIntervalle_combined<rawHit_DIF, rawHit_IsInIntervalle<rawHit_Amplitude> > (3,7, rawHit_IsInIntervalle<rawHit_Amplitude>(1) ) )  ==2);
  
  assert(std::count_if(hitvec.begin(),hitvec.end(), rawHit_IsInIntervalle_combined<rawHit_DIF, rawHit_IsInIntervalle_combined<rawHit_ASIC,  rawHit_IsEqual<rawHit_Channel> > >(3, rawHit_IsInIntervalle_combined<rawHit_ASIC,  rawHit_IsEqual<rawHit_Channel> >(1, rawHit_IsEqual<rawHit_Channel>(14)) ) )==1);
  assert(std::count_if(hitvec.begin(),hitvec.end(), rawHit_IsInIntervalle_combined<rawHit_DIF, rawHit_IsInIntervalle_combined<rawHit_ASIC,  rawHit_IsEqual<rawHit_Channel> > >(3, rawHit_IsInIntervalle_combined<rawHit_ASIC,  rawHit_IsEqual<rawHit_Channel> >(1, rawHit_IsEqual<rawHit_Channel>(15)) ) )==0);
  
  assert(std::count_if(hitvec.begin(),hitvec.end(), timed_rawHit_IsInIntervalle<rawHit_DIF>(UI_intervalle(3,3),UI_intervalle(5,6)))==2); 

  return 0;
}
