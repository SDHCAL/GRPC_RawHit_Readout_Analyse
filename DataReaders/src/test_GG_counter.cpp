// test program

#include <assert.h>





#include "GG_counter.h"


int main()
{


  SingleCounter countA;
  countA.add(1);   assert(countA.sumcount() == 1); assert(countA.flagcount()==1);
  countA.add(3);   assert(countA.sumcount() == 4); assert(countA.flagcount()==1);
  countA.newSet(); assert(countA.sumcount() == 4); assert(countA.flagcount()==1);
  countA.add(2);   assert(countA.sumcount() == 6); assert(countA.flagcount()==2);
  std::cout << "countA"; countA.print();
 
  unsigned int valeur=1;
  unsigned int param[3];
  MappedCounters<SingleCounter> channelCount;
  valeur=1; param[2]=4; channelCount.add(valeur, param+2); channelCount.newSet();
  valeur=1; channelCount.add(valeur, param+2); param[2]=6; channelCount.add(valeur, param+2);
  assert(channelCount.sumcount()==3);
  assert(channelCount.flagcount()==2);
  assert(channelCount.size()==2);
  std::map<unsigned int,SingleCounter>::iterator ita=channelCount.begin();
  assert(ita->first==4);
  assert(ita->second.sumcount()==2);
  assert(ita->second.flagcount()==2);
  ++ita;
  assert(ita->first==6);
  assert(ita->second.sumcount()==1);
  assert(ita->second.flagcount()==1);
  std::string labels[2]={"all Channels", "channel"};
  channelCount.print(labels);


  MappedCounters<MappedCounters<SingleCounter> > asicchannelcount;
  valeur=1; param[2]=4; param[1]=1;  asicchannelcount.add(valeur, param+1); //[1][4] = (1/1)
  assert(asicchannelcount.begin()->first==1);
  assert(asicchannelcount[1].begin()->first==4);
  param[1]=2;  asicchannelcount.add(valeur, param+1);   //[1][4] = (1/1) [2][4] = (1/1)
  assert(asicchannelcount.begin()->first==1);
  assert(asicchannelcount[1].begin()->first==4);
  assert(asicchannelcount.size()==2);
  assert(asicchannelcount[2].begin()->first==4);
  asicchannelcount.newSet();
  valeur=3; param[2]=5; param[1]=1;  asicchannelcount.add(valeur, param+1); //[1][4] = (1/1) [1][5] = (3/1) [2][4] = (1/1)
  assert(asicchannelcount.size()==2);
  std::map<unsigned int,MappedCounters<SingleCounter> >::iterator itb=asicchannelcount.begin();
  assert(itb->first==1);
  assert(itb->second.size()==2);
  assert(itb->second[4].sumcount()==1);
  assert(itb->second[4].flagcount()==1);
  assert(itb->second[5].sumcount()==3);
  assert(itb->second[5].flagcount()==1);
  assert(itb->second.sumcount()==4);
  assert(itb->second.flagcount()==2);
  ++itb;
  assert(itb->first==2);
  assert(itb->second.size()==1);
  assert(itb->second[4].sumcount()==1);
  assert(itb->second[4].flagcount()==1);
  assert(itb->second.sumcount()==1);
  assert(itb->second.flagcount()==1);
  assert(asicchannelcount.sumcount()==5);
  assert(asicchannelcount.flagcount()==2);
  std::string asiclabels[3]={"all ASICs", "ASIC", "channel"};
  asicchannelcount.print(asiclabels);

  MappedCounters<MappedCounters<MappedCounters<SingleCounter> > > difasichannelcount;
  valeur=1; param[2]=4; param[1]=1; param[0]=1; difasichannelcount.add(valeur, param);
  valeur=3; param[2]=4; param[1]=1; param[0]=2; difasichannelcount.add(valeur, param);
  difasichannelcount.newSet();
  valeur=1; param[2]=4; param[1]=1; param[0]=1; difasichannelcount.add(valeur, param);
  valeur=1; param[2]=4; param[1]=2; param[0]=1; difasichannelcount.add(valeur, param);
  difasichannelcount.newSet();
  valeur=1; param[2]=4; param[1]=1; param[0]=1; difasichannelcount.add(valeur, param);
  valeur=1; param[2]=5; param[1]=2; param[0]=1; difasichannelcount.add(valeur, param);
  std::string difasiclabels[4]={"all DIFs", "DIF", "ASIC", "channel"};
  difasichannelcount.print(difasiclabels);

  return 0;
}
