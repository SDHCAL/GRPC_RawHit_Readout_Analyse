#ifndef ExperimentalSetup_HH
#define ExperimentalSetup_HH

#include <set>
#include <vector>
#include <map>
#include <iostream>

#include "DIFdrivenDevice.h"
#include "PadDevice.h"
#include "BifaceStripDevice.h"

class ExperimentalSetup
{
 private:
  ExperimentalSetup(const ExperimentalSetup&) {;}
  void operator=(const ExperimentalSetup&) {;}
 public:
  typedef unsigned int DIFNUMBER;
 ExperimentalSetup() : m_BIFs(), m_plans(), m_padDeviceDIFMap(), m_stripDeviceDIFMap() {;}

  ~ExperimentalSetup();

  void addBIF(DIFNUMBER bif) {if (DIFnumberIsKnown(bif)) message(bif); else m_BIFs.insert(bif);}
  bool hasBIF() const {return ! m_BIFs.empty();}
  DIFNUMBER getBIF() const { return *(m_BIFs.begin());} // Assume only one BIF per experimental setup, to rethink if one day we put 2 BIFs during a testbeam

  bool DIFnumberIsBIF(DIFNUMBER dif) const {return (! m_BIFs.empty()) && m_BIFs.find(dif)!=m_BIFs.end();}
  bool DIFnumberIsStrip(DIFNUMBER dif) const {return (! m_stripDeviceDIFMap.empty()) && m_stripDeviceDIFMap.find(dif) != m_stripDeviceDIFMap.end();}
  bool DIFnumberIsPad(DIFNUMBER dif) const {return (! m_padDeviceDIFMap.empty()) && m_padDeviceDIFMap.find(dif) != m_padDeviceDIFMap.end();}
  bool DIFnumberIsKnown(DIFNUMBER dif) const {return (DIFnumberIsBIF(dif) || DIFnumberIsStrip(dif) || DIFnumberIsPad(dif) );}

  bool PlanIsStrip(unsigned int planNumber) const;
  bool PlanIsPad(unsigned int planNumber) const;

  void addOneDIFPadDevice(DIFNUMBER dif);
  void addSDHCALPlan(DIFNUMBER dif_J0, DIFNUMBER dif_J32, DIFNUMBER dif_J64);
  void addCMSstrip(DIFNUMBER EvenStripDIF, DIFNUMBER OddStripDIF);
  
  std::vector<DIFNUMBER> getStripDevice_DIFnumber();
  std::vector<DIFNUMBER> getPadDevice_DIFnumber();
  std::vector<DIFdrivenDevice*> getPlans() {return m_plans;}

  PadDevice* getPadDevice(DIFNUMBER dif) { return (DIFnumberIsPad(dif) ? m_padDeviceDIFMap[dif] : NULL);}
  BifaceStripDevice* getStripDevice(DIFNUMBER dif) { return (DIFnumberIsStrip(dif) ? m_stripDeviceDIFMap[dif] : NULL);}
  DIFdrivenDevice& getOrAddDevice(DIFNUMBER dif);  //Warning : will crash if dif is a BIF

  void getCoord3D(unsigned int dif,unsigned int asic, unsigned int channel, unsigned int& I, unsigned int& J, unsigned int & K); 

  unsigned int nPlans() const {return m_plans.size();}
  unsigned int getPlanNumber(unsigned int dif) {return getOrAddDevice(dif).getK();}
 private:
  std::set<DIFNUMBER> m_BIFs;
  std::vector<DIFdrivenDevice*> m_plans;
  std::map<DIFNUMBER, PadDevice*> m_padDeviceDIFMap;
  std::map<DIFNUMBER, BifaceStripDevice*> m_stripDeviceDIFMap;

  void message(DIFNUMBER dif) {std::cout << "WARNING : DIF number " << dif << " allready used " << std::endl;}
};


#endif
