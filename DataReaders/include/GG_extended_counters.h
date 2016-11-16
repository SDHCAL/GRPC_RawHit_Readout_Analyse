#ifndef GG_extended_counter
#define GG_extended_counter

#include "GG_counter.h"
#include <vector>

template <class COUNTER>
class ArrayCounter 
{
public:
  ArrayCounter(unsigned int theSize) : m_counters(theSize) {;}
  void add(unsigned int val, unsigned int *keys) { m_counters[keys[0]].add(val,keys+1);}
  void add(unsigned int val, unsigned int indice, unsigned int *keys) {m_counters[indice].add(val,keys);}
  void newSet() {for (typename std::vector<COUNTER>::iterator it=m_counters.begin(); it!= m_counters.end(); ++it) it->newSet();}
  void print(std::string* labels,std::ostream& oflux=std::cout) 
  {
    for (unsigned int i=0; i<m_counters.size();++i)
      {oflux << labels[0] << " " << i << " : "; m_counters[i].print(labels+1,oflux);}
  }
  void ASCIIwrite(std::ostream& oflux=std::cout) const
  {
    oflux << m_counters.size() << " "; 
    for (typename std::vector<COUNTER>::const_iterator it=m_counters.begin(); it!= m_counters.end(); ++it) it->ASCIIwrite(oflux);
  }
  void ASCIIread(std::istream& iflux=std::cin)
  {
    unsigned int theSize; 
    iflux>>theSize;
    m_counters=std::vector<COUNTER>(theSize);
    for (unsigned int i=0; i<theSize; ++i) m_counters[i].ASCIIread(iflux);
  }
  unsigned int size() {return m_counters.size();}
  COUNTER getCounter(unsigned int i) {return m_counters[i];}
private:
  std::vector<COUNTER> m_counters;
};


template <class COUNTER>
class ArrayUpToCounter : public  ArrayCounter<COUNTER>
{
public:
  ArrayUpToCounter(unsigned int theSize) : ArrayCounter<COUNTER>(theSize) {;}
  void add(unsigned int val, unsigned int uptoindice, unsigned int *keys) {for (unsigned int i=0; i<=uptoindice; ++i) ArrayCounter<COUNTER>::add(val,i,keys);}
  void add(unsigned int val, unsigned int *keys) { ArrayUpToCounter<COUNTER>::add(val,keys[0],keys+1); }
};


template <class COUNTER>
class RunThresholdCounter
{
public:
  //kind of structure 
  unsigned int n_event;
  unsigned int nLevels() const {return COUNTER::printIndentLevel+2;}
  std::string labels[COUNTER::printIndentLevel+2];
  ArrayUpToCounter<COUNTER> m_thresholdCounters;

  RunThresholdCounter() : m_thresholdCounters(3), n_event(0) {;}

  void print(std::ostream& oflux=std::cout) { oflux << n_event << " events " << std::endl; m_thresholdCounters.print(labels,oflux);}
  void ASCIIwrite(std::ostream& oflux=std::cout) const
  {
    oflux << nLevels() << " ";
    for (unsigned int i=0; i<nLevels(); ++i) oflux << labels[i] << " ";
    oflux << n_event<< " ";
    m_thresholdCounters.ASCIIwrite(oflux);
  }
  bool ASCIIread(std::istream& iflux=std::cin)
  {
    std::streampos before=iflux.tellg();
    unsigned int level; iflux >> level;
    if (level != nLevels() )
      { 
	std::cout << "ERROR reading run Threshold counters read level " << level << " is not expected level " << nLevels() << std::endl;
	iflux.seekg(before);
	return false;
      }
    for (unsigned int i=0; i<nLevels(); ++i) iflux >> labels[i];
    iflux >> n_event;
    m_thresholdCounters.ASCIIread(iflux);
    return true;
  }
private:
};

typedef RunThresholdCounter<DIFCounters> RunThresholdCounter_DifAsicChannel;  //3 level counters [dif][asic][channel]
typedef RunThresholdCounter<AsicCounters> RunThresholdCounter_PlaneGap;       //2 level counters [dif][asic] or [plan][gap] or [asic][channel]

#endif
