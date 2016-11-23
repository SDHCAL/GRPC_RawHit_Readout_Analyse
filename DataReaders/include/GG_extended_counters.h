#ifndef GG_extended_counter
#define GG_extended_counter

#include "GG_counter.h"
#include <vector>
#include <algorithm>

template <class COUNTER>
class ArrayCounter : public ASCIIpersistance
{
public:
  ArrayCounter(unsigned int theSize) : m_counters(theSize) {;}
  void add(unsigned int val, unsigned int *keys) { m_counters[keys[0]].add(val,keys+1);}
  void add(unsigned int val, unsigned int indice, unsigned int *keys) {m_counters[indice].add(val,keys);}
  void newSet() {for (typename std::vector<COUNTER>::iterator it=m_counters.begin(); it!= m_counters.end(); ++it) it->newSet();}
  void write(std::string* labels,std::ostream& oflux=std::cout) 
  {
    for (unsigned int i=0; i<m_counters.size();++i)
      {oflux << labels[0] << " " << i << " : "; m_counters[i].write(labels+1,oflux);}
  }
  bool ASCIIwrite(std::ostream& oflux=std::cout) const
  {
    oflux << m_counters.size() << " "; 
    for (typename std::vector<COUNTER>::const_iterator it=m_counters.begin(); it!= m_counters.end(); ++it) it->ASCIIwrite(oflux);
    return oflux.good();
  }
  bool ASCIIread(std::istream& iflux=std::cin)
  {
    unsigned int theSize; 
    iflux>>theSize;
    m_counters=std::vector<COUNTER>(theSize);
    for (unsigned int i=0; i<theSize; ++i) m_counters[i].ASCIIread(iflux);
    return iflux.good();
  }
  unsigned int size() {return m_counters.size();}
  COUNTER& getCounter(unsigned int i) {return m_counters[i];}
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
class RunThresholdCounter : public ASCIIpersistance
{
public:
  //kind of structure 
  unsigned int n_event;
  unsigned int nLevels() const {return COUNTER::printIndentLevel+2;}
  ArrayUpToCounter<COUNTER> m_thresholdCounters;

 RunThresholdCounter() : n_event(0), m_thresholdCounters(3) { for (unsigned int i=0; i<nLevels(); ++i) m_labels[i]="Unset_label";}

  void setLabel(unsigned int i, std::string label) 
  {std::replace(label.begin(),label.end(),' ','_');if (label.empty()) label="Unset_label"; m_labels[i]=label;}
  std::string getLabel(unsigned int i) const {return m_labels[i];}

  void write(std::ostream& oflux=std::cout) { oflux << n_event << " events " << std::endl; m_thresholdCounters.write(m_labels,oflux);}
  bool ASCIIwrite(std::ostream& oflux=std::cout) const
  {
    oflux << nLevels() << " ";
    for (unsigned int i=0; i<nLevels(); ++i) oflux << m_labels[i] << " ";
    oflux << n_event<< " ";
    m_thresholdCounters.ASCIIwrite(oflux);
    return oflux.good();
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
    for (unsigned int i=0; i<nLevels(); ++i) iflux >> m_labels[i];
    iflux >> n_event;
    m_thresholdCounters.ASCIIread(iflux);
    return iflux.good();
  }
private:
  std::string m_labels[COUNTER::printIndentLevel+2];
};

typedef RunThresholdCounter<DIFCounters> RunThresholdCounter_DifAsicChannel;  //3 level counters [dif][asic][channel]
typedef RunThresholdCounter<AsicCounters> RunThresholdCounter_PlaneGap;       //2 level counters [dif][asic] or [plan][gap] or [asic][channel]

#endif
