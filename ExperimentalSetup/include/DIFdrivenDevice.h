#ifndef DIFdrivenDevice_HH
#define DIFdrivenDevice_HH

#include <utility>
#include <vector>
#include <algorithm>
#include "Plane_position.h"

class RawCalorimeterHitPointer;

class DIFdrivenDevice
{
 public:
  typedef std::pair<unsigned int, unsigned int> I_J;  //standard pad
  typedef std::pair<unsigned int, unsigned int> strip_gap; //CMS strip
  typedef strip_gap strip_angle; // Tricot
  struct dif_asic_channel
  {
    unsigned int dif;
    unsigned int asic;
    unsigned int channel;
    dif_asic_channel(unsigned int d, unsigned int a, unsigned int c) : dif(d), asic(a), channel(c) {}
  };
  
  DIFdrivenDevice(unsigned int layer=0) : m_layerNumber(layer), m_DIFnumbers() {;}
  virtual ~DIFdrivenDevice() {;}

  //un plan = 3 DIF ou 1 DIF
  //une strip CMS = 2 DIF
  void addDIF(unsigned int difnumber) {if ( ! isDrivenBy(difnumber) ) m_DIFnumbers.push_back(difnumber);}

  bool isDrivenBy(unsigned int difnumber) const {return std::find( m_DIFnumbers.begin(), m_DIFnumbers.end(), difnumber) != m_DIFnumbers.end();}

  virtual unsigned int maxNumberOfASICs() const=0;
  
  //Pad electronics
  virtual  I_J getIJ(unsigned int dif, unsigned int asic, unsigned int channel) const=0;

  //strip electronics
  strip_gap getStripnumberGapnumber(unsigned int dif, unsigned int asic, unsigned int channel) const {return getIJ(dif,asic,channel);}
  strip_angle getStripnumberAnglenumber(unsigned int dif, unsigned int asic, unsigned int channel) const {return getIJ(dif,asic,channel);}

  //reverse translation
  virtual dif_asic_channel getDifAsicChannel(unsigned int IorStrip, unsigned int JorGaporAngle) const=0; 
  
  //layer number
  unsigned int getK() const {return m_layerNumber;}
  unsigned int getLayerNumber() const {return getK();}

  std::vector<unsigned int> DIFnumbers() {return m_DIFnumbers;}

  void setPlanePosition(const Plane_position& plan) {m_ouSuisJe=plan;}
  virtual bool planPositionIsImplemented() const {return false;}
  virtual std::pair<float,float> getPositionInPlanIn_mm(unsigned int I, unsigned int J, const std::vector<RawCalorimeterHitPointer>& hits) const {return std::make_pair(0.0,0.0);}
  void getAbsolutePositionIn_mm(unsigned int I, unsigned int J, const std::vector<RawCalorimeterHitPointer>& hits, float* pos) const
  {auto xIJ=getPositionInPlanIn_mm(I,J,hits); m_ouSuisJe.getPosition(xIJ.first,xIJ.second,pos);}
 protected:
  unsigned int m_layerNumber;
  std::vector<unsigned int> m_DIFnumbers;
  Plane_position m_ouSuisJe;
};

#endif
