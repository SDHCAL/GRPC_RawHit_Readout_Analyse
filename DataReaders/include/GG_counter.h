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
