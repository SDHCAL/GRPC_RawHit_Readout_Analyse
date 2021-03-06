#ifndef RawCalorimeterHitClustering_HH
#define RawCalorimeterHitClustering_HH

#include "RawCalorimeterHitPointer.h"
#include "Clustering.h" 

#include <map>
#include <iterator>

//Cluster of Raw hits
typedef SDHCAL::Cluster<RawCalorimeterHitPointer> RawHitCluster;
typedef std::vector<RawHitCluster> RawHitClustersVec; 

//print functions

inline std::ostream& operator<<(std::ostream& outFlux, const RawHitCluster& cluster)
{
  for (std::set<const RawCalorimeterHitPointer*>::const_iterator it=cluster.begin(); it!= cluster.end(); ++it)
    outFlux << (**it) << "---";
  return outFlux;
}

inline std::ostream& operator<<(std::ostream& outFlux, const RawHitClustersVec& clustervec)
{
  for (std::vector<RawHitCluster>::const_iterator it=clustervec.begin(); it!=clustervec.end(); ++it)
    outFlux << "{{{" <<  *it << "}}}";
  return outFlux;
}

std::ostream& operator<<(std::ostream& outFlux, const std::vector<std::vector<RawCalorimeterHitPointer>::iterator>& clusterBounds);

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
float mean(const RawHitCluster& cluster, RawHitGetter f)
{
  if (cluster.empty()) return 0;
  float sum=0;
  for (std::set<const RawCalorimeterHitPointer*>::const_iterator it=cluster.begin(); it != cluster.end(); ++it)
    sum+=float(f.get(**it));
  return sum/cluster.size();
}
template <class RawHitGetter>
float mean(const RawHitCluster& cluster)
{
  return mean(cluster,RawHitGetter());
}
template <class RawHitGetter>
float mean(const std::vector<RawCalorimeterHitPointer>::iterator& begin, const std::vector<RawCalorimeterHitPointer>::iterator& end, RawHitGetter f)
{
  float sum=0;
  for (std::vector<RawCalorimeterHitPointer>::iterator it=begin; it != end; ++it)
    sum+=float(f.get(*it));
  return sum/std::distance(begin,end);
}

template <class RawHitGetter>
std::map<unsigned int, unsigned int> distribution(const RawHitCluster& cluster,RawHitGetter f)
{
  std::map<unsigned int, unsigned int> m;
  for (std::set<const RawCalorimeterHitPointer*>::const_iterator it=cluster.begin(); it != cluster.end(); ++it)
    m[f.get(**it)]++;
  return m;
}
template <class RawHitGetter>
std::map<unsigned int, unsigned int> distribution(const RawHitCluster& cluster)
{
  return distribution(cluster,RawHitGetter());
}

template <class RawHitGetter>
std::map<unsigned int, unsigned int> distribution(const std::vector<RawCalorimeterHitPointer>::iterator& begin, const std::vector<RawCalorimeterHitPointer>::iterator& end, RawHitGetter f)
{
  std::map<unsigned int, unsigned int> m;
  for (std::vector<RawCalorimeterHitPointer>::iterator it=begin; it != end; ++it)
    m[f.get(*it)]++;
  return m;
}

unsigned int mostFrequentValue(std::map<unsigned int, unsigned int> &m);


template <class RawHitGetter>
unsigned int mostFrequentValue(const RawHitCluster& cluster,RawHitGetter f)
{
  std::map<unsigned int, unsigned int> m=distribution(cluster,f);
  return mostFrequentValue(m);
}
template <class RawHitGetter>
unsigned int mostFrequentValue(const RawHitCluster& cluster)
{
  return distribution(cluster,RawHitGetter());
}
template <class RawHitGetter>
unsigned int mostFrequentValue(const std::vector<RawCalorimeterHitPointer>::iterator& begin, const std::vector<RawCalorimeterHitPointer>::iterator& end, RawHitGetter f)
{
  std::map<unsigned int, unsigned int> m=distribution(begin,end,f);
  return mostFrequentValue(m);
}

#endif
