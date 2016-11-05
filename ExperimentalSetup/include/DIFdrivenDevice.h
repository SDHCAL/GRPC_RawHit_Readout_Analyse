#ifndef DIFdrivenDevice_HH
#define DIFdrivenDevice_HH

#include <utility>
#include <vector>
#include <algorithm>

class DIFdrivenDevice
{
 public:
  typedef std::pair<unsigned int, unsigned int> I_J;
  typedef std::pair<unsigned int, unsigned int> strip_gap;

  DIFdrivenDevice(unsigned int layer=0) : m_layerNumber(layer), m_DIFnumbers() {;}
  virtual ~DIFdrivenDevice() {;}

  //un plan = 3 DIF ou 1 DIF
  //une strip CMS = 2 DIF
  void addDIF(unsigned int difnumber) {if ( ! isDrivenBy(difnumber) ) m_DIFnumbers.push_back(difnumber);}

  bool isDrivenBy(unsigned int difnumber) const {return std::find( m_DIFnumbers.begin(), m_DIFnumbers.end(), difnumber) != m_DIFnumbers.end();}

  //Pad electronics
  virtual  I_J getIJ(unsigned int dif, unsigned int asic, unsigned int channel) const=0;

  //strip electronics
  strip_gap getStripnumberGapnumber(unsigned int dif, unsigned int asic, unsigned int channel) const {return getIJ(dif,asic,channel);}

  //layer number
  unsigned int getK() const {return m_layerNumber;}
  unsigned int getLayerNumber() const {return getK();}

  std::vector<unsigned int> DIFnumbers() {return m_DIFnumbers;}

 protected:
  unsigned int m_layerNumber;
  std::vector<unsigned int> m_DIFnumbers; 
};

#endif
