#ifndef GG_counter_HH
#define GG_counter_HH

#include <map>
#include <iostream>
#include <string>
#include "ASCIIpersistance.h"

class SingleCounter : public ASCIIpersistance
{
 public:
 SingleCounter() : m_count(0), m_flagcount(0), m_flag(false) {;}

  void add(unsigned int val=1, unsigned int *unused=NULL) {m_count+=val;  if (!m_flag) {++m_flagcount;m_flag=true;}}
  void newSet() {m_flag=false;}
  
  unsigned int sumcount() const {return m_count;}
  unsigned int flagcount() const {return m_flagcount;}

  void write(std::string* labels=NULL,std::ostream& oflux=std::cout) { oflux <<  (labels==NULL ? std::string("") : (*labels))  << " : " <<  m_count << " for " << m_flagcount << std::endl;}

  bool ASCIIwrite(std::ostream& oflux=std::cout) const { oflux << m_count << " " << m_flagcount<< " " ; return oflux.good();}
  bool ASCIIread(std::istream& iflux=std::cin) {iflux >> m_count >> m_flagcount; return iflux.good();}

  bool operator==(const SingleCounter& other) const {return sumcount()==other.sumcount() && flagcount()==other.flagcount();}
  bool operator!=(const SingleCounter& other) const {return ! ((*this)==other);}

  static const unsigned int printIndentLevel=0;

  SingleCounter& counterAtLevel(unsigned int level, unsigned int *) {return *this;} 

 private:
  unsigned int m_count;
  unsigned int m_flagcount;
  bool m_flag;
};

//COUNTER should derive form COUNTERBASE
template <class COUNTER, class COUNTERBASE=SingleCounter>
class MappedCounters : public std::map<unsigned int,COUNTER>, public COUNTERBASE
{
 public:
  void add(unsigned int val, unsigned int *keys) {COUNTERBASE::add(val); (*this)[keys[0]].add(val,keys+1); }
  void newSet() {COUNTERBASE::newSet(); for (typename std::map<unsigned int,COUNTER>::iterator it=this->begin(); it!= this->end(); ++it) it->second.newSet();}
 
  COUNTERBASE& counterAtLevel(unsigned int level, unsigned int *keys) 
    {
      if (level>=printIndentLevel) return COUNTERBASE::counterAtLevel(level,keys);
      return (*this)[keys[0]].counterAtLevel(level,keys+1);
    }

  void write(std::string* labels,std::ostream& oflux=std::cout) 
  {
    COUNTERBASE::write(labels,oflux); 
    for (typename std::map<unsigned int,COUNTER>::iterator it=this->begin(); it!= this->end(); ++it) {printIndent(oflux); oflux << it->first << " "; it->second.write(labels+1,oflux);}
  }

  bool ASCIIwrite(std::ostream& oflux=std::cout) const
  { 
    COUNTERBASE::ASCIIwrite(oflux);  
    oflux << this->size() << " ";
    for (typename std::map<unsigned int,COUNTER>::const_iterator it=this->begin(); it!= this->end(); ++it) 
      { oflux << it->first << " "; it->second.ASCIIwrite(oflux); }
    return oflux.good();
  }
  bool ASCIIread(std::istream& iflux=std::cin)
  {
    this->clear();
    COUNTERBASE::ASCIIread(iflux);
    unsigned int mapsize;
    iflux >> mapsize;
    for (unsigned int i=0; i<mapsize; ++i)
      { unsigned int aKey; iflux >> aKey; (*this)[aKey].ASCIIread(iflux); }
    return iflux.good();
  }

  const std::map<unsigned int,COUNTER>& the_map() const {return *this;}
  bool operator==(const MappedCounters<COUNTER>& other) const {return  COUNTERBASE::operator==(other) && the_map()==other.the_map();}
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


#endif
