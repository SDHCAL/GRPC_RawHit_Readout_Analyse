#ifndef ExperimentalSetup_HH
#define ExperimentalSetup_HH

#include <set>
#include <vector>
#include <map>
#include <iostream>
#include <iterator>

#include "DIFdrivenDevice.h"
#include "PadDevice.h"
#include "BifaceStripDevice.h"
#include "TricotDevice.h"

class ExperimentalSetup
{
 private:
  ExperimentalSetup(const ExperimentalSetup&) {;}
  void operator=(const ExperimentalSetup&) {;}
 public:
  typedef unsigned int DIFNUMBER;
 ExperimentalSetup() : m_BIFs(), m_plans(), m_padDeviceDIFMap(), m_stripDeviceDIFMap(), m_tricotDeviceDIFMap()  {;}

  ~ExperimentalSetup();

  void addBIF(DIFNUMBER bif) {if (DIFnumberIsKnown(bif)) message(bif); else m_BIFs.insert(bif);}
  bool hasBIF() const {return ! m_BIFs.empty();}
  DIFNUMBER getBIF(unsigned int which=0) const { std::set<DIFNUMBER>::const_iterator it=m_BIFs.begin(); std::advance(it,which); return which<m_BIFs.size() ? *it : 0;}
  
  
  bool DIFnumberIsBIF(DIFNUMBER dif) const {return (! m_BIFs.empty()) && m_BIFs.find(dif)!=m_BIFs.end();}
  bool DIFnumberIsStrip(DIFNUMBER dif) const {return (! m_stripDeviceDIFMap.empty()) && m_stripDeviceDIFMap.find(dif) != m_stripDeviceDIFMap.end();}
  bool DIFnumberIsPad(DIFNUMBER dif) const {return (! m_padDeviceDIFMap.empty()) && m_padDeviceDIFMap.find(dif) != m_padDeviceDIFMap.end();}
  bool DIFnumberIsTricot(DIFNUMBER dif) const {return (! m_tricotDeviceDIFMap.empty()) && m_tricotDeviceDIFMap.find(dif) != m_tricotDeviceDIFMap.end();}
  bool DIFnumberIsKnown(DIFNUMBER dif) const {return (DIFnumberIsBIF(dif) || DIFnumberIsStrip(dif) || DIFnumberIsPad(dif) || DIFnumberIsTricot(dif) );}

  bool PlanIsStrip(unsigned int planNumber) const;
  bool PlanIsPad(unsigned int planNumber) const;
  bool PlanIsTricot(unsigned int planNumber) const;
  

  void addOneDIFPadDevice(DIFNUMBER dif,Plane_position plan=Plane_position());
  void addSDHCALPlan(DIFNUMBER dif_J0, DIFNUMBER dif_J32, DIFNUMBER dif_J64,Plane_position plan=Plane_position());
  void addCMSstrip(DIFNUMBER EvenStripDIF, DIFNUMBER OddStripDIF,Plane_position plan=Plane_position());
  void addTricot(DIFNUMBER dif, Plane_position plan=Plane_position(), unsigned int nAngles=3);
  
  std::vector<DIFNUMBER> getStripDevice_DIFnumber() const;
  std::vector<DIFNUMBER> getPadDevice_DIFnumber() const;
  std::vector<DIFNUMBER> getTricotDevice_DIFnumber() const;
  std::vector<DIFdrivenDevice*> getPlans() const {return m_plans;}

  PadDevice* getPadDevice(DIFNUMBER dif) { return (DIFnumberIsPad(dif) ? m_padDeviceDIFMap[dif] : NULL);}
  BifaceStripDevice* getStripDevice(DIFNUMBER dif) { return (DIFnumberIsStrip(dif) ? m_stripDeviceDIFMap[dif] : NULL);}
  TricotDevice* getTricotDevice(DIFNUMBER dif) { return (DIFnumberIsTricot(dif) ? m_tricotDeviceDIFMap[dif] : NULL);}
  DIFdrivenDevice& getOrAddDevice(DIFNUMBER dif);  //Warning : will crash if dif is a BIF

  void getCoord3D(unsigned int dif,unsigned int asic, unsigned int channel, unsigned int& I, unsigned int& J, unsigned int & K); 
  void getDAQ_ID(unsigned int I, unsigned int J, unsigned int K, unsigned int &dif,unsigned int &asic, unsigned int &channel);
  
  unsigned int nBIFs() const {return m_BIFs.size();}
  unsigned int nPlans() const {return m_plans.size();}
  unsigned int getPlanNumber(unsigned int dif) {return getOrAddDevice(dif).getK();} //Warning : will crash if dif is a BIF
  unsigned int getPlanNumberSafe(unsigned int dif) {return (DIFnumberIsKnown(dif) and not DIFnumberIsBIF(dif)) ? getOrAddDevice(dif).getK() : 1000000;}
 private:
  std::set<DIFNUMBER> m_BIFs;
  std::vector<DIFdrivenDevice*> m_plans;
  std::map<DIFNUMBER, PadDevice*> m_padDeviceDIFMap;
  std::map<DIFNUMBER, BifaceStripDevice*> m_stripDeviceDIFMap;
  std::map<DIFNUMBER, TricotDevice*> m_tricotDeviceDIFMap;

  void message(DIFNUMBER dif) {std::cout << "WARNING : DIF number " << dif << " allready used " << std::endl;}
  template <class DEVICE>
    bool PlanIsDevice(unsigned int planNumber, const std::map<DIFNUMBER, DEVICE*>& m) const;
  template <class DEVICE>
    std::vector<DIFNUMBER> getDIFnumber(const std::map<DIFNUMBER, DEVICE*>& m) const;
};


#endif
