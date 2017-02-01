#ifndef RawCalorimeterHitClustering_HH
#define RawCalorimeterHitClustering_HH

#include "RawCalorimeterHitPointer.h"
#include "Clustering.h" 

//Cluster of Raw hits
typedef SDHCAL::Cluster<RawCalorimeterHitPointer> RawHitCluster;
typedef std::list<RawHitCluster> RawHitClustersList; 

//function pointer typedef
typedef bool (*RawHitMergeFunction)(const RawCalorimeterHitPointer&,const RawCalorimeterHitPointer&);

//functors for predicate
class defaultRawHitMerge
{
 public:
  inline bool operator()(const RawCalorimeterHitPointer &A,const RawCalorimeterHitPointer &B)
  {
    return A.dif() == B.dif() && abs(A->getTimeStamp()-B->getTimeStamp())<2 && abs(int(A.I_local())-int(B.I_local()))<2 && abs(int(A.J_local())-int(B.J_local()))<2;
  }
};

class defaultRawHitStripMerge
{
 public:
  inline bool operator()(const RawCalorimeterHitPointer &A,const RawCalorimeterHitPointer &B)
  {
    return A.dif() == B.dif() and A.asic() == B.asic() and abs(int(A.channel())-int(B.channel()))<2 && abs(A->getTimeStamp()-B->getTimeStamp())<2;
  }
};

#endif
