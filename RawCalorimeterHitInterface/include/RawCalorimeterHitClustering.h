#ifndef RawCalorimeterHitClustering_HH
#define RawCalorimeterHitClustering_HH

#include "RawCalorimeterHitPointer.h"
#include "Clustering.h" 

#include <map>

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


//template function using functors defined in RawCalorimeterHitPointerTools.h
template <class RawHitGetter>
float mean(const RawHitCluster& cluster)
{
  if (cluster.empty()) return 0;
  float sum=0;
  RawHitGetter f;
  for (std::set<const RawCalorimeterHitPointer*>::const_iterator it=cluster.begin(); it != cluster.end(); ++it)
    sum+=float(f.get(**it));
  return sum/cluster.size();
}

template <class RawHitGetter>
std::map<unsigned int, unsigned int> distribution(const RawHitCluster& cluster)
{
  std::map<unsigned int, unsigned int> m;
  RawHitGetter f;
  for (std::set<const RawCalorimeterHitPointer*>::const_iterator it=cluster.begin(); it != cluster.end(); ++it)
    m[f.get(**it)]++;
  return m;
}

unsigned int mostFrequentValue(std::map<unsigned int, unsigned int> &m);


template <class RawHitGetter>
unsigned int mostFrequentValue(const RawHitCluster& cluster)
{
  std::map<unsigned int, unsigned int> m=distribution<RawHitGetter>(cluster);
  return mostFrequentValue(m);
}

#endif
