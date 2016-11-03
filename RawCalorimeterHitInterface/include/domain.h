#ifndef domain_HH
#define domain_HH

#include "intervalle.h"

#include <vector>
#include <algorithm>

///////////////////////////////////////////////////////////
//
// helper function to check if a value is in a list of intervalles
//
///////////////////////////////////////////////////////////
template <class T>
inline bool valueIn(const std::vector<T>& values, T searchedValue)
{
  return std::find( values.begin(), values.end(), searchedValue) != values.end();
}


template <class T>
inline bool valueIn(const std::vector<intervalle<T> >& values, T searchedValue)
{
  return std::find_if( values.begin(), values.end(), contains<T>(searchedValue)) != values.end();
}

///////////////////////////////////////////////////////////
//
// the domain class
//
///////////////////////////////////////////////////////////

template<class T>
class domain
{
 public:
 domain(const std::vector<intervalle<T> >& intervals=std::vector<intervalle<T> >(), 
	const std::vector<T> singleValues=std::vector<T>()) : m_intervals(intervals), m_singleValues(singleValues) {;}
 domain(const std::vector<T> singleValues) : m_intervals(), m_singleValues(singleValues) {;}
 domain(intervalle<T> interval) : m_intervals(1,interval), m_singleValues() {;}
 domain(T value) : m_intervals(),  m_singleValues(1,value) {;}

 bool valueInDomain(T searchedValue) const {return valueIn(m_intervals,searchedValue) || valueIn(m_singleValues,searchedValue);}

 void addValue(T value) {m_singleValues.push_back(value);}
 void addIntervalle(intervalle<T> I) {m_intervals.push_back(I);}

 private:
  std::vector<intervalle<T> > m_intervals;  
  std::vector<T> m_singleValues;
};

typedef domain<unsigned int> UI_domain;
typedef domain<float> F_domain;

class aboveFirstThreshold  : public UI_domain { public: aboveFirstThreshold()  : domain(1) {addValue(2); addValue(3);} };
class aboveSecondThreshold : public UI_domain { public: aboveSecondThreshold() : domain(1) {addValue(3);} };
class aboveThirdThreshold  : public UI_domain { public: aboveThirdThreshold()  : domain(3) {;} };

#endif
