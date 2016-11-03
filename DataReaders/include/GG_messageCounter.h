#ifndef GG_messageCounter_HH
#define GG_messageCounter_HH

#include <string>
#include <iostream>


class GG_messageCounter
{
 public:
  GG_messageCounter(std::string message, std::ostream &outflux=std::cout) : m_outflux(&outflux), m_message(message), m_nPrintedMessage(0), m_maxPrintedMessage(10) {;}
  ~GG_messageCounter() { if (m_nPrintedMessage>0) (*m_outflux) << m_message << " printed " << m_nPrintedMessage << " times." << std::endl;}

  void setMaxPrintedMessage(unsigned int value) {m_maxPrintedMessage=value;}
  void setOutputFlux(std::ostream &outflux) {m_outflux=&outflux;}
  bool quiet() {return m_nPrintedMessage>m_maxPrintedMessage;}
  void print() {++m_nPrintedMessage; if (!quiet()) (*m_outflux) << m_message << std::endl;}

 private:
  std::ostream *m_outflux;
  std::string m_message;
  unsigned int m_nPrintedMessage;
  unsigned int m_maxPrintedMessage;
};

#endif
