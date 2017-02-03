// test program

#include <assert.h>

#include "RawCalorimeterHitClustering.h"
#include "RawCalorimeterHitUtilities.h"
#include "RawHit_SDHCAL_Data.h"
#include "RawCalorimeterHitPointerTools.h"

#include <iostream>

bool sameDifAsic(const RawCalorimeterHitPointer &A,const RawCalorimeterHitPointer &B)
{
  return A.dif() == B.dif() and A.asic() == B.asic();
}

bool sameDif(const RawCalorimeterHitPointer &A,const RawCalorimeterHitPointer &B)
{
  return A.dif() == B.dif();
}

class channelClose
{
public:
  channelClose(unsigned int val) : m_val(val) {}
  bool operator()(const RawCalorimeterHitPointer &A,const RawCalorimeterHitPointer &B)
  {
    return abs(int(A.channel())-int(B.channel()))<=m_val;
  }
private:
  unsigned int m_val;
};

int main()
{
    std::vector<RawCalorimeterHitPointer> hitvec;
// int                          amplitude, timestamp, dif, asic, channel
// un cluster temps-channel
  hitvec.push_back(createRawHit(        1,         5,   7,    1,      13));
  hitvec.push_back(createRawHit(        1,         6,   7,    1,      14));
  hitvec.push_back(createRawHit(        1,         6,   7,    1,      13));
  hitvec.push_back(createRawHit(        1,         7,   7,    1,      14));
// un (ou plusieurs) cluster temps-channel
  hitvec.push_back(createRawHit(        1,       102,   7,    2,      33));
  hitvec.push_back(createRawHit(        1,       102,   7,    2,      34));
  hitvec.push_back(createRawHit(        1,       102,   7,    2,      35));
  hitvec.push_back(createRawHit(        1,       102,   7,    2,      36));
  hitvec.push_back(createRawHit(        1,       102,   7,    2,      37));
  hitvec.push_back(createRawHit(        1,       102,   7,    2,      39));
  hitvec.push_back(createRawHit(        1,       102,   7,    2,      42));
  hitvec.push_back(createRawHit(        1,       102,   7,    2,      46));
  // le meme que le premier avec un numero de dif different
  hitvec.push_back(createRawHit(        1,         5,  55,    1,      13));
  hitvec.push_back(createRawHit(        1,         6,  55,    1,      14));
  hitvec.push_back(createRawHit(        1,         6,  55,    1,      13));
  hitvec.push_back(createRawHit(        1,         7,  55,    1,      14));


  RawHit_SDHCAL_Data d(hitvec,111,12,357698);
  assert(not d.hasCluster());
  d.clusterize(defaultRawHitStripMerge());
  assert(d.hasCluster());
    
  RawHitClustersList clusters=d.getClusters();
  std::cout << clusters << std::endl;
  
  assert(clusters.size()==6);
  RawHitClustersList::iterator first=clusters.begin();
  assert(first->size()==4); 
  assert(mean<rawHit_DIF>(*first)==7);
  assert(mean<rawHit_ASIC>(*first)==1);
  assert(mean<rawHit_Channel>(*first)==13.5);
  assert(mean<rawHit_TimeStamp>(*first)==6);

  std::map<unsigned int, unsigned int> m=distribution<rawHit_DIF>(*first);
  assert(m.size()==1);
  assert(m.begin()->first==7);
  assert(m.begin()->second==4);
  assert(mostFrequentValue(m)==7);

  m=distribution<rawHit_Channel>(*first);
  assert(m.size()==2);
  assert(m.begin()->first==13);
  assert(m.begin()->second==2);
  assert(m.rbegin()->first==14);
  assert(m.rbegin()->second==2);
  assert(mostFrequentValue(m)==13);
  
  m=distribution<rawHit_TimeStamp>(*first);
  assert(m.size()==3);
  assert(m.begin()->first==5);
  assert(m.begin()->second==1);
  assert(m.rbegin()->first==7);
  assert(m.rbegin()->second==1);
  assert(mostFrequentValue(m)==6);

  ++first;
  assert(first->size()==5); 
  ++first;
  assert(first->size()==1); 
  assert((*(first->begin()))->dif()==7);
  assert((*(first->begin()))->asic()==2);
  assert((*(first->begin()))->channel()==39);

  ++first;
  assert(first->size()==1); 
  assert((*(first->begin()))->channel()==42);

  ++first;
  assert(first->size()==1); 
  assert((*(first->begin()))->channel()==46);

  ++first;
  assert(first->size()==4); 
  assert((*(first->begin()))->dif()==55);

  d.clusterize(sameDifAsic);
  std::cout << d.getClusters() << std::endl;
  assert(d.getClusters().size()==3);

  d.clusterize(sameDif);
  std::cout << d.getClusters() << std::endl;
  assert(d.getClusters().size()==2);

  
  d.clusterize(channelClose(1));
  assert(d.getClusters().size()==5);
  d.clusterize(channelClose(2));
  assert(d.getClusters().size()==4);
  d.clusterize(channelClose(3));
  assert(d.getClusters().size()==3);
  d.clusterize(channelClose(4));
  assert(d.getClusters().size()==2);


  std::cout << d.getClusters()  << std::endl;
	       
  return 0;
}
