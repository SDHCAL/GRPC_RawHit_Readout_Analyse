// test program

#include <assert.h>

#include "RawCalorimeterHitClustering.h"
#include "RawCalorimeterHitUtilities.h"
#include "RawHit_SDHCAL_Data.h"
#include "RawCalorimeterHitPointerTools.h"
#include "RawHit_standard_merge_predicate.h"
#include "ExperimentalSetup.h"

#include <iostream>


int main()
{
  ExperimentalSetup setup;
  setup.addBIF(3);
  setup.addOneDIFPadDevice(12);
  setup.addOneDIFPadDevice(13);
  setup.addSDHCALPlan(36,37,38);
  setup.addCMSstrip(67,68);

  RawHit_standard_merge_predicate pred(setup);
  
  std::vector<RawCalorimeterHitPointer> hitvec;
// int                          amplitude, timestamp, dif, asic, channel
  hitvec.push_back(createRawHit(        1,         5,   3,    1,      13)); //BIF
  hitvec.push_back(createRawHit(        1,         8,  12,   11,      27)); //PAD I=20, J=12
  hitvec.push_back(createRawHit(        1,         8,  12,   11,      28)); //PAD I=20, J=13
  hitvec.push_back(createRawHit(        1,         8,  12,   11,      36)); //PAD I=21, J=12  //these 3 forms a cluster
  hitvec.push_back(createRawHit(        1,         8,  12,   11,      21)); //PAD I=19, J=11  //is in previous cluster if padJunctionByCorner is allowed
  hitvec.push_back(createRawHit(        1,        12,  12,   11,      26)); //PAD I=20, J=11  //will retrieve previous hit in cluster if padJunctionByCorner disabled and time intervalle increased enough
  


  RawHit_SDHCAL_Data d(hitvec,111,12,357698);
  d.clusterize(pred);
  
  RawHitClustersList clusters=d.getClusters();
  std::cout << clusters << std::endl;
  assert(clusters.size()==4);
  
  //first cluster should be BIF one
  assert(clusters.front().size()==1);
  assert((*clusters.front().begin())->dif()==3);

 //second cluster should be PAD one with 3 hits
  clusters.pop_front(); 
  assert(clusters.front().size()==3);
  assert((*clusters.front().begin())->dif()==12);
  assert((*clusters.front().begin())->I_local()==20);
  assert((*clusters.front().begin())->J_local()==12);
  assert((*clusters.front().rbegin())->I_local()==21);
  assert((*clusters.front().rbegin())->J_local()==12);

 //third cluster should be PAD one with 1 hit
  clusters.pop_front(); 
  assert(clusters.front().size()==1);
  assert((*clusters.front().begin())->dif()==12);
  assert((*clusters.front().begin())->I_local()==19);
  assert((*clusters.front().begin())->J_local()==11);

 //fourth cluster should be PAD one with 1 hit
  clusters.pop_front(); 
  assert(clusters.front().size()==1);
  assert((*clusters.front().begin())->dif()==12);
  assert((*clusters.front().begin())->I_local()==20);
  assert((*clusters.front().begin())->J_local()==11);

  pred.setPadJunctionByCorner(true);
  d.clusterize(pred);
  clusters=d.getClusters();
  std::cout << clusters << std::endl;
  assert(clusters.size()==3);
  //second clusters should have 4 hits with last one being I,J=19,11
  clusters.pop_front(); 
  assert(clusters.front().size()==4);
  assert((*clusters.front().begin())->dif()==12);
  assert((*clusters.front().rbegin())->I_local()==19);
  assert((*clusters.front().rbegin())->J_local()==11);
 //last cluster should be the same as in previous case
  clusters.pop_front(); 
  assert(clusters.front().size()==1);
  assert((*clusters.front().begin())->dif()==12);
  assert((*clusters.front().begin())->I_local()==20);
  assert((*clusters.front().begin())->J_local()==11);

  pred.setPadJunctionByCorner(false);
  pred.setNeighbourTimeDistance(10);
  d.clusterize(pred);
  clusters=d.getClusters();
  std::cout << clusters << std::endl;
  assert(clusters.size()==2);
  //last cluster should have 5 hits with last one being I,J=20,11 
  assert(clusters.back().size()==5);
  assert((*clusters.back().rbegin())->dif()==12);
  assert((*clusters.back().rbegin())->I_local()==20);
  assert((*clusters.back().rbegin())->J_local()==11);
  
  pred.setNeighbourTimeDistance(1);
  d.clusterize(pred);
  clusters=d.getClusters();
  assert(clusters.size()==4);

  //ASIC corner
  hitvec.push_back(createRawHit(        1,         8,  12,   7,      46)); //PAD I=16, J=24
  hitvec.push_back(createRawHit(        1,         8,  12,   8,      52)); //PAD I=16, J=25
  hitvec.push_back(createRawHit(        1,         8,  12,   9,      12)); //PAD I=17, J=25
  hitvec.push_back(createRawHit(        1,         8,  12,  10,      17)); //PAD I=17, J=24
  d.replaceVec(hitvec);
  d.clusterize(pred);
  clusters=d.getClusters();
  std::cout << clusters << std::endl;
  assert(clusters.size()==5);

  //different Plane doesn't cluster
  hitvec.push_back(createRawHit(        1,         8,  13,   7,      46)); //PAD I=16, J=24
  d.replaceVec(hitvec);
  d.clusterize(pred);
  clusters=d.getClusters();
  std::cout << clusters << std::endl;
  assert(clusters.size()==6);
  assert(clusters.back().size()==1);
  assert((*clusters.back().rbegin())->dif()==13);
  
  //ASU separation
  hitvec.push_back(createRawHit(        1,         8,  36,   8,      18)); //PAD I=10, J=32
  hitvec.push_back(createRawHit(        1,         8,  36,   8,      20)); //PAD I=11, J=32
  hitvec.push_back(createRawHit(        1,         8,  37,   5,      23)); //PAD I=11, J=33
  d.replaceVec(hitvec);
  d.clusterize(pred);
  clusters=d.getClusters();
  std::cout << clusters << std::endl;
  assert(clusters.size()==7);
  assert(clusters.back().size()==3);
  std::map<unsigned int, unsigned int> m=distribution<rawHit_DIF>(clusters.back());
  assert(m.size()==2);
  assert(m[36]==2);
  assert(m[37]==1);
  assert(m.size()==2);

  //strip clustering
  pred.setNeighbourStripDistance(1);
  hitvec.push_back(createRawHit(        1,         8,  67,   1,      10)); //Strip=20
  hitvec.push_back(createRawHit(        1,         8,  67,   1,      11)); //Strip=22
  hitvec.push_back(createRawHit(        1,         8,  68,   1,      53)); //Strip=21
  d.replaceVec(hitvec);
  d.clusterize(pred);
  clusters=d.getClusters();
  std::cout << clusters << std::endl;
  assert(clusters.size()==8);
  assert(clusters.back().size()==3);

  //different gap makes different clusters
  pred.setNeighbourStripDistance(2);
  hitvec.push_back(createRawHit(        1,         8,  67,   2,      10)); //Strip=20
  hitvec.push_back(createRawHit(        1,         8,  67,   2,      11)); //Strip=22
  d.replaceVec(hitvec);
  d.clusterize(pred);
  clusters=d.getClusters();
  std::cout << clusters << std::endl;
  assert(clusters.size()==9);
  assert(clusters.back().size()==2);

  std::cout << hitvec.size() << std::endl;
  return 0;
}
