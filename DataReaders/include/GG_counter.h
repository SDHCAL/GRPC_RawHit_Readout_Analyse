#ifndef GG_counter
#define GG_counter

#include <map>
#include <iostream>
#include <string>

class SingleCounter
{
 public:
 SingleCounter() : m_count(0), m_flagcount(0), m_flag(false) {;}

  void add(unsigned int val=1, unsigned int *unused=NULL) {m_count+=val;  if (!m_flag) {++m_flagcount;m_flag=true;}}
  void newSet() {m_flag=false;}
  
  unsigned int sumcount() const {return m_count;}
  unsigned int flagcount() const {return m_flagcount;}
  void print(std::string* labels=NULL,std::ostream& oflux=std::cout) { oflux <<  (labels==NULL ? std::string("") : (*labels))  << " : " <<  m_count << " for " << m_flagcount << std::endl;}

  void ASCIIwrite(std::ostream& oflux=std::cout) const { oflux << m_count << " " << m_flagcount<< " " ; }
  void ASCIIread(std::istream& iflux=std::cin) {iflux >> m_count >> m_flagcount;}

  bool operator==(const SingleCounter& other) const {return sumcount()==other.sumcount() && flagcount()==other.flagcount();}
  bool operator!=(const SingleCounter& other) const {return ! ((*this)==other);}

  static const unsigned int printIndentLevel=0;

 private:
  unsigned int m_count;
  unsigned int m_flagcount;
  bool m_flag;
};

template <class COUNTER>
class MappedCounters : public std::map<unsigned int,COUNTER>, public SingleCounter
{
 public:
  void add(unsigned int val, unsigned int *keys) {SingleCounter::add(val); (*this)[keys[0]].add(val,keys+1); }
  void newSet() {SingleCounter::newSet(); for (typename std::map<unsigned int,COUNTER>::iterator it=this->begin(); it!= this->end(); ++it) it->second.newSet();}
  void print(std::string* labels,std::ostream& oflux=std::cout) 
  {
    SingleCounter::print(labels,oflux); 
    for (typename std::map<unsigned int,COUNTER>::iterator it=this->begin(); it!= this->end(); ++it) {printIndent(oflux); oflux << it->first << " "; it->second.print(labels+1,oflux);}
  }

  void ASCIIwrite(std::ostream& oflux=std::cout) const
  { 
    SingleCounter::ASCIIwrite(oflux);  
    oflux << this->size() << " ";
    for (typename std::map<unsigned int,COUNTER>::const_iterator it=this->begin(); it!= this->end(); ++it) 
      { oflux << it->first << " "; it->second.ASCIIwrite(oflux); }
  }
  void ASCIIread(std::istream& iflux=std::cin)
  {
    this->clear();
    SingleCounter::ASCIIread(iflux);
    unsigned int mapsize;
    iflux >> mapsize;
    for (unsigned int i=0; i<mapsize; ++i)
      { unsigned int aKey; iflux >> aKey; (*this)[aKey].ASCIIread(iflux); }
  }

  const std::map<unsigned int,COUNTER>& the_map() const {return *this;}
  bool operator==(const MappedCounters<COUNTER>& other) const {return  SingleCounter::operator==(other) && the_map()==other.the_map();}
  bool operator!=(const MappedCounters<COUNTER>& other) const {return ! ((*this)==other);}

  static const unsigned int printIndentLevel=COUNTER::printIndentLevel+1;
 private:
  static void printIndent(std::ostream& oflux) { for (unsigned int i=printIndentLevel; i<5;  ++i) oflux << "  ";}
  
}; 

typedef MappedCounters<SingleCounter> ChannelCounters;
typedef MappedCounters<ChannelCounters> AsicCounters;
typedef MappedCounters<AsicCounters> DIFCounters;

typedef MappedCounters<SingleCounter> ICounters;
typedef MappedCounters<ICounters> JCounters;
typedef MappedCounters<JCounters> PlaneCounters;


template <class COUNTER>
class extendedMappedCounters : public MappedCounters<COUNTER>
{
 public:
  unsigned int n_event;
  std::string labels[MappedCounters<COUNTER>::printIndentLevel+1];
  unsigned int nLevels() const {return MappedCounters<COUNTER>::printIndentLevel+1;}

  void print(std::ostream& oflux=std::cout) { oflux << n_event << " events " << std::endl; MappedCounters<COUNTER>::print(labels,oflux);}
  void ASCIIwrite(std::ostream& oflux=std::cout) const
  {
    oflux << nLevels() << " ";
    for (unsigned int i=0; i<nLevels(); ++i) oflux << labels[i] << " ";
    oflux << n_event<< " ";
    MappedCounters<COUNTER>::ASCIIwrite(oflux);
  }
  bool ASCIIread(std::istream& iflux=std::cin)
  {
    std::streampos before=iflux.tellg();
    unsigned int level; iflux >> level;
    if (level != nLevels() )
      { 
	std::cout << "ERROR reading extended Mapped counters read level " << level << " is not expected level " << nLevels() << std::endl;
	iflux.seekg(before);
	return false;
      }
    for (unsigned int i=0; i<nLevels(); ++i) iflux >> labels[i];
    iflux >> n_event;
    MappedCounters<COUNTER>::ASCIIread(iflux);
    return true;
  }
  void operator=(const MappedCounters<COUNTER>& counter) 
    {
      *((std::map<unsigned int,COUNTER>*) this)=counter.the_map();
      *((SingleCounter*) this)=*((SingleCounter*) &counter);
    }
};

#endif
