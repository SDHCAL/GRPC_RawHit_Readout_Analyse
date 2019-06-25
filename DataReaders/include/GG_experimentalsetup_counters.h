#ifndef GG_experimentalsetup_counters_HH
#define GG_experimentalsetup_counters_HH

#include "GG_extended_counters.h"
#include "ExperimentalSetup.h"

class TDirectory;
class TH2F;

class GG_ExperimentalSetup_Counter : public ArrayCounter<DIFCounters>
{
 public:
  GG_ExperimentalSetup_Counter(ExperimentalSetup *setup) : m_setup(setup), ArrayCounter<DIFCounters>(NULL==setup ? 1 : 2) {;}
  void add(unsigned int val, unsigned int dif, unsigned int asic, unsigned int channel);
  void write(std::ostream& oflux=std::cout);
  void saveTo(TDirectory* ROOTdir,std::string histoBaseName);
 private:
  ExperimentalSetup *m_setup;

  void setAxisTitle(TH2F* Histo, std::string Yname, std::string Xname);
  void setAxisTitle(std::pair<TH2F*,TH2F*> &Histos, std::string Yname, std::string Xname) {setAxisTitle(Histos.first,Yname,Xname); setAxisTitle(Histos.second,Yname,Xname);}
    
};

#endif

