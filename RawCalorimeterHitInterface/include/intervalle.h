#ifndef intervalle_HH
#define intervalle_HH

#include <utility>
#include <iostream>


///////////////////////////////////////////////////////////
//
//  Template class definition
//
///////////////////////////////////////////////////////////

template <class NOMBRE>
class intervalle : public std::pair<NOMBRE,NOMBRE>
{
  //NOMBRE is float or int depending of the case
  //In the pair it is assumed that first <= second
 public:
  intervalle() : std::pair<NOMBRE,NOMBRE>(0,1) {;}
  intervalle(NOMBRE a, NOMBRE b) : std::pair<NOMBRE,NOMBRE>(a,b) {if (a>b) swap();} 
  void expand(NOMBRE a, NOMBRE b) { if (a<this->first) this->first=a; if (b>this->second) this->second=b;}
  NOMBRE length() {return this->second-this->first;}
  bool CorrectlyOrdered() {return this->first<= this->second;}
 private:
  void swap() {NOMBRE a=this->first; this->first=this->second; this->second=a;}
};

template <class NOMBRE>
inline std::ostream& operator<<(std::ostream& flux, const intervalle<NOMBRE>& i)
{
  flux << "["<<i.first<<","<<i.second<<"]"; return flux;
}

///////////////////////////////////////////////////////////
//
//  Typedef for the main cases
//
///////////////////////////////////////////////////////////
typedef intervalle<unsigned int> UI_intervalle;
typedef intervalle<float> F_intervalle;

///////////////////////////////////////////////////////////
//
// functor to check if a value is in an intervalle
//
///////////////////////////////////////////////////////////
template <class T>
class contains
{
 public:
 contains(T value=0) : m_value(value) {;}
  bool operator()(const intervalle<T>& I) {return (m_value >= I.first) && (m_value<=I.second);}
 private:
  T m_value;
};



#endif
