#ifndef RawCalorimeterHitPointerTools_HH
#define RawCalorimeterHitPointerTools_HH

#include "RawCalorimeterHitPointer.h"

#include "domain.h"

#include <iterator>
#include <algorithm>

inline void print(const std::vector<RawCalorimeterHitPointer>& hitvec, std::ostream& outFlux)
{
  std::ostream_iterator<RawCalorimeterHitPointer> out_it (outFlux,"\n");
  std::copy ( hitvec.begin(), hitvec.end(), out_it );
}

//
// extract sub vectors
//T is an object defining unsigned int T::get(const RawCalorimeterHitPointer &)
//
template <class T>
std::vector<RawCalorimeterHitPointer> extract(const std::vector<RawCalorimeterHitPointer>& vec, const UI_domain& D, T what, bool notInverted=true)
{
  std::vector<RawCalorimeterHitPointer> retour;
  for (std::vector<RawCalorimeterHitPointer>::const_iterator it=vec.begin(); it != vec.end(); ++it)
    if ( D.valueInDomain(what.get(*it))==notInverted )
      retour.push_back(*it);
  return retour;
}

template <class T> 
std::vector<RawCalorimeterHitPointer> extract(const std::vector<RawCalorimeterHitPointer>& vec, const UI_intervalle& I, T what, bool notInverted=true)
{return extract(vec,UI_domain(I),what,notInverted);}

template <class T> 
std::vector<RawCalorimeterHitPointer> extract(const std::vector<RawCalorimeterHitPointer>& vec, const unsigned int& I, T what, bool notInverted=true)
{return extract(vec,UI_domain(I),what,notInverted);}



//functors
struct rawHit_TimeStamp { unsigned int get(const RawCalorimeterHitPointer &h) {return h->getTimeStamp();} };
struct rawHit_Amplitude { unsigned int get(const RawCalorimeterHitPointer &h) {return h->getAmplitude();} };  
struct rawHit_Threshold { unsigned int get(const RawCalorimeterHitPointer &h) {return h.thresholdCrossed();} };
struct rawHit_DIF       { unsigned int get(const RawCalorimeterHitPointer &h) {return h.dif();} };
struct rawHit_ASIC      { unsigned int get(const RawCalorimeterHitPointer &h) {return h.asic();} };
struct rawHit_Channel   { unsigned int get(const RawCalorimeterHitPointer &h) {return h.channel();} };
struct rawHit_Ilocal    { unsigned int get(const RawCalorimeterHitPointer &h) {return h.I_local();} };
struct rawHit_Jlocal    { unsigned int get(const RawCalorimeterHitPointer &h) {return h.J_local();} };
struct rawHit_EvenStrip { unsigned int get(const RawCalorimeterHitPointer &h) {return h.stripNumberEven();} };
struct rawHit_OddStrip  { unsigned int get(const RawCalorimeterHitPointer &h) {return h.stripNumberOdd();} };

template <class T>
struct rawHit_IsEqual 
{ 
public:
  rawHit_IsEqual(unsigned int value) : m_value(value) {;}
  bool operator()(const RawCalorimeterHitPointer &h) {T truc; return truc.get(h)==m_value;}
private:
  unsigned int m_value;
};

template <class T>
struct rawHit_IsInIntervalle
{ 
public:
  rawHit_IsInIntervalle(UI_intervalle inter) : m_intervalle(inter) {;}
rawHit_IsInIntervalle(unsigned int minvalue,unsigned int maxvalue) : m_intervalle(UI_intervalle(minvalue,maxvalue)) {;}
  rawHit_IsInIntervalle(unsigned int singlevalue) : m_intervalle(UI_intervalle(singlevalue,singlevalue)) {;}
  bool operator()(const RawCalorimeterHitPointer &h) {T truc; return truc.get(h)>=m_intervalle.first && truc.get(h)<=m_intervalle.second;}
private:
  UI_intervalle m_intervalle;
};

template <class T, class TSUB>
struct rawHit_IsInIntervalle_combined 
{
public:
  rawHit_IsInIntervalle_combined(UI_intervalle inter, TSUB sub) : m_intervalle(inter), m_sub(sub) {;}
  rawHit_IsInIntervalle_combined(unsigned int minvalue, unsigned int maxvalue, TSUB sub) : m_intervalle(UI_intervalle(minvalue,maxvalue)), m_sub(sub) {;}
  rawHit_IsInIntervalle_combined(unsigned int singlevalue, TSUB sub) : m_intervalle(UI_intervalle(singlevalue,singlevalue)), m_sub(sub) {;}
  bool operator()(const RawCalorimeterHitPointer &h) {T truc; return truc.get(h)>=m_intervalle.first && truc.get(h)<=m_intervalle.second && m_sub(h);}
private:
  UI_intervalle m_intervalle;
  TSUB m_sub;
};

template <class T> 
struct timed_rawHit_IsInIntervalle : public rawHit_IsInIntervalle_combined<T,rawHit_IsInIntervalle<rawHit_TimeStamp > >
{
 public:
 timed_rawHit_IsInIntervalle(UI_intervalle value_intervalle, UI_intervalle time_intervalle) : rawHit_IsInIntervalle_combined<T,rawHit_IsInIntervalle<rawHit_TimeStamp > >(value_intervalle,rawHit_IsInIntervalle<rawHit_TimeStamp >(time_intervalle)) {;}
};

#endif
