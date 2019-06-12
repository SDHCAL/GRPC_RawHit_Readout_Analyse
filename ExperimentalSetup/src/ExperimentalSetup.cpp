#include "ExperimentalSetup.h"


ExperimentalSetup::~ExperimentalSetup()
{
  for (std::vector<DIFdrivenDevice*>::iterator it=m_plans.begin(); it!=m_plans.end(); ++it)
    delete (*it);
}

template <class DEVICE>
bool ExperimentalSetup::PlanIsDevice(unsigned int planNumber, const std::map<DIFNUMBER, DEVICE*>& m) const
{
  if (planNumber>= m_plans.size()) return false;
  DIFdrivenDevice* searched=m_plans[planNumber];
  for (auto pm : m)
    if (pm.second==searched) return true;
  return false;
}

bool ExperimentalSetup::PlanIsStrip(unsigned int planNumber) const {return PlanIsDevice(planNumber,m_stripDeviceDIFMap);}
bool ExperimentalSetup::PlanIsPad(unsigned int planNumber) const {return PlanIsDevice(planNumber,m_padDeviceDIFMap);}
bool ExperimentalSetup::PlanIsTricot(unsigned int planNumber) const {return PlanIsDevice(planNumber,m_tricotDeviceDIFMap);}


void ExperimentalSetup::addOneDIFPadDevice(DIFNUMBER dif, Plane_position plan)  
{
  if (DIFnumberIsKnown(dif)) {message(dif); return;}
  PadDevice* p = new PadDevice(dif, m_plans.size());
  p->setPlanePosition(plan);
  m_plans.push_back(p);
  m_padDeviceDIFMap[dif]=p;
}

void ExperimentalSetup::addSDHCALPlan(DIFNUMBER dif_J0, DIFNUMBER dif_J32, DIFNUMBER dif_J64, Plane_position plan)
{
  if (DIFnumberIsKnown(dif_J0)) {message(dif_J0); return;}
  if (DIFnumberIsKnown(dif_J32)) {message(dif_J32); return;}
  if (DIFnumberIsKnown(dif_J64)) {message(dif_J64); return;}
  PadDevice* p = new PadDevice(dif_J0, dif_J32, dif_J64, m_plans.size());
  p->setPlanePosition(plan);
  m_plans.push_back(p);
  m_padDeviceDIFMap[dif_J0]=p;  
  m_padDeviceDIFMap[dif_J32]=p;  
  m_padDeviceDIFMap[dif_J64]=p;  
}

void ExperimentalSetup::addCMSstrip(DIFNUMBER EvenStripDIF, DIFNUMBER OddStripDIF, Plane_position plan)
{
  if (DIFnumberIsKnown(EvenStripDIF)) {message(EvenStripDIF); return;}
  if (DIFnumberIsKnown(OddStripDIF)) {message(OddStripDIF); return;}
  BifaceStripDevice* p = new BifaceStripDevice(EvenStripDIF,OddStripDIF,m_plans.size());
  p->setPlanePosition(plan);
  m_plans.push_back(p);
  m_stripDeviceDIFMap[EvenStripDIF]=p;
  m_stripDeviceDIFMap[OddStripDIF]=p;
}

void ExperimentalSetup::addTricot(DIFNUMBER dif, Plane_position plan, unsigned int nAngles)
{
  if (DIFnumberIsKnown(dif)) {message(dif); return;}
  TricotDevice* p = new TricotDevice(dif, m_plans.size(), nAngles);
  p->setPlanePosition(plan);
  m_plans.push_back(p);
  m_tricotDeviceDIFMap[dif]=p;

}

template <class DEVICE>
std::vector<ExperimentalSetup::DIFNUMBER> ExperimentalSetup::getDIFnumber(const std::map<DIFNUMBER, DEVICE*>& m) const
{
  std::vector<DIFNUMBER> vec;
  for (auto pm : m) vec.push_back(pm.first);
  return vec;
}


std::vector<ExperimentalSetup::DIFNUMBER> ExperimentalSetup::getStripDevice_DIFnumber() const { return getDIFnumber(m_stripDeviceDIFMap);}
std::vector<ExperimentalSetup::DIFNUMBER> ExperimentalSetup::getPadDevice_DIFnumber() const {return getDIFnumber(m_padDeviceDIFMap);}
std::vector<ExperimentalSetup::DIFNUMBER> ExperimentalSetup::getTricotDevice_DIFnumber() const {return getDIFnumber(m_tricotDeviceDIFMap);}


DIFdrivenDevice& ExperimentalSetup::getOrAddDevice(DIFNUMBER dif)
{
  if (DIFnumberIsBIF(dif)) {abort();}
  if (!DIFnumberIsKnown(dif))
    {
      std::cout << "WARNING : Adding in a new layer an unexpected DIF number : " << dif << std::endl;
      addOneDIFPadDevice(dif);
      return *(m_plans[m_plans.size()-1]);
    }
  if (DIFnumberIsPad(dif)) return *(m_padDeviceDIFMap[dif]);
  if (DIFnumberIsTricot(dif)) return *(m_tricotDeviceDIFMap[dif]);
  return *(m_stripDeviceDIFMap[dif]);
}


void ExperimentalSetup::getCoord3D(unsigned int dif,unsigned int asic, unsigned int channel, unsigned int& I, unsigned int& J, unsigned int & K)
{
  if (DIFnumberIsBIF(dif)) return;
  DIFdrivenDevice& device=getOrAddDevice(dif);
  std::pair<unsigned int, unsigned int> IJ=device.getIJ(dif,asic,channel);
  I=IJ.first;
  J=IJ.second;
  K=device.getK();
}

void ExperimentalSetup::getDAQ_ID(unsigned int I, unsigned int J, unsigned int K, unsigned int &dif,unsigned int &asic, unsigned int &channel)
{
  if (K>=m_plans.size()) return;
  DIFdrivenDevice::dif_asic_channel DAQ_ID=m_plans[K]->getDifAsicChannel(I,J);
  dif=DAQ_ID.dif;
  asic=DAQ_ID.asic;
  channel=DAQ_ID.channel;
}
