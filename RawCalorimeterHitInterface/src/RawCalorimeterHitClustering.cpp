#include "RawCalorimeterHitClustering.h"


unsigned int mostFrequentValue(std::map<unsigned int, unsigned int> &m)
{
  if (m.empty()) return 0;
  std::map<unsigned int, unsigned int>::iterator itmax=m.begin();
  for (std::map<unsigned int, unsigned int>::iterator it=m.begin(); it!= m.end(); ++it)
    if (it->second > itmax->second)
      itmax=it;
  return itmax->first;
}

std::ostream& operator<<(std::ostream& outFlux, const std::vector<std::vector<RawCalorimeterHitPointer>::iterator>& clusterBounds)
{
  for (unsigned int i=0; i!=clusterBounds.size()-1; ++i)
    {
      outFlux << "{{{";
      for (std::vector<RawCalorimeterHitPointer>::iterator it=clusterBounds[i]; it != clusterBounds[i+1]; ++it)
	outFlux << (*it) << "---";
      outFlux << "}}}";
    }
  return outFlux;
}
