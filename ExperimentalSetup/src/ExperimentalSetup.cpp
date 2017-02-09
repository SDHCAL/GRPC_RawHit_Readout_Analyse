#include "ExperimentalSetup.h"


ExperimentalSetup::~ExperimentalSetup()
{
  for (std::vector<DIFdrivenDevice*>::iterator it=m_plans.begin(); it!=m_plans.end(); ++it)
    delete (*it);
}

bool ExperimentalSetup::PlanIsStrip(unsigned int planNumber) const
{
  if (planNumber>= m_plans.size()) return false;
  DIFdrivenDevice* searched=m_plans[planNumber];
  for (std::map<DIFNUMBER, BifaceStripDevice*>::const_iterator itmap=m_stripDeviceDIFMap.begin(); itmap!=m_stripDeviceDIFMap.end(); ++itmap)
    if (itmap->second==searched) return true;
  return false;
}

bool ExperimentalSetup::PlanIsPad(unsigned int planNumber) const
{
  if (planNumber>= m_plans.size()) return false;
  DIFdrivenDevice* searched=m_plans[planNumber];
  for (std::map<DIFNUMBER, PadDevice*>::const_iterator itmap=m_padDeviceDIFMap.begin(); itmap!=m_padDeviceDIFMap.end(); ++itmap)
    if (itmap->second==searched) return true;
  return false;
}


void ExperimentalSetup::addOneDIFPadDevice(DIFNUMBER dif)  
{
  if (DIFnumberIsKnown(dif)) {message(dif); return;}
  PadDevice* p = new PadDevice(dif, m_plans.size());
  m_plans.push_back(p);
  m_padDeviceDIFMap[dif]=p;
}

void ExperimentalSetup::addSDHCALPlan(DIFNUMBER dif_J0, DIFNUMBER dif_J32, DIFNUMBER dif_J64)
{
  if (DIFnumberIsKnown(dif_J0)) {message(dif_J0); return;}
  if (DIFnumberIsKnown(dif_J32)) {message(dif_J32); return;}
  if (DIFnumberIsKnown(dif_J64)) {message(dif_J64); return;}
  PadDevice* p = new PadDevice(dif_J0, dif_J32, dif_J64, m_plans.size());
  m_plans.push_back(p);
  m_padDeviceDIFMap[dif_J0]=p;  
  m_padDeviceDIFMap[dif_J32]=p;  
  m_padDeviceDIFMap[dif_J64]=p;  
}

void ExperimentalSetup::addCMSstrip(DIFNUMBER EvenStripDIF, DIFNUMBER OddStripDIF)
{
  if (DIFnumberIsKnown(EvenStripDIF)) {message(EvenStripDIF); return;}
  if (DIFnumberIsKnown(OddStripDIF)) {message(OddStripDIF); return;}
  BifaceStripDevice* p = new BifaceStripDevice(EvenStripDIF,OddStripDIF,m_plans.size());
  m_plans.push_back(p);
  m_stripDeviceDIFMap[EvenStripDIF]=p;
  m_stripDeviceDIFMap[OddStripDIF]=p;
}

std::vector<unsigned int > ExperimentalSetup::getStripDevice_DIFnumber()
{
  std::vector<DIFNUMBER> vec;
  for ( std::map<DIFNUMBER, BifaceStripDevice*>::iterator it=m_stripDeviceDIFMap.begin(); it != m_stripDeviceDIFMap.end(); ++it) vec.push_back(it->first);
  return vec;
}

std::vector<unsigned int> ExperimentalSetup::getPadDevice_DIFnumber()
{
  std::vector<DIFNUMBER> vec;
  for ( std::map<DIFNUMBER, PadDevice*> ::iterator it=m_padDeviceDIFMap.begin(); it!=m_padDeviceDIFMap.end(); ++it) vec.push_back(it->first);
  return vec;
}


DIFdrivenDevice& ExperimentalSetup::getOrAddDevice(DIFNUMBER dif)
{
  if (DIFnumberIsBIF(dif)) {abort();}
  if (!DIFnumberIsKnown(dif)) {addOneDIFPadDevice(dif); return *(m_plans[m_plans.size()-1]);}
  if (DIFnumberIsPad(dif)) return *(m_padDeviceDIFMap[dif]);
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
