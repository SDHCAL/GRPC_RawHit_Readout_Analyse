// test program

#include <assert.h>

#include "intervalle.h"
#include "domain.h"


int main()
{
  //check intervalle
  //check contains
  UI_intervalle A,B(3,5),C(8,2);
  contains<unsigned int> val(3);

  assert(val(A)==false);
  assert(val(B));
  assert(val(C));


  UI_intervalle D(330,450); std::cout << " D:" << D; assert(D.first==330); assert(D.second==450); //[330,450]
  D.expand(12,16); std::cout << " " << D;            assert(D.first==12); assert(D.second==450);  // [12,450] 
  D.expand(20,50); std::cout << " " << D;            assert(D.first==12); assert(D.second==450);  // [12,450] 
  D.expand(100,500); std::cout << " " << D;          assert(D.first==12); assert(D.second==500);  // [12,500] 
  D.expand(4,513); std::cout << " " << D;            assert(D.first==4); assert(D.second==513);   //  [4,513] 
  std::cout<<std::endl;
  // should print the line :
  // " D:[330,450] [12,450] [12,450] [12,500] [4,513]"

  //check domain valueIn
  std::vector<UI_intervalle> vec; 
  vec.push_back(A);
  vec.push_back(B);
  vec.push_back(C);
  assert(valueIn(vec,18u)==false);
  assert(valueIn(vec,8u));
  assert(valueIn(vec,4u));


  UI_domain da(vec); assert(da.valueInDomain(18)==false);
  da.addValue(18);   assert(da.valueInDomain(18));

  UI_domain db(UI_intervalle(5,8));       assert(db.valueInDomain(18)==false);
  db.addIntervalle(UI_intervalle(9,27));  assert(db.valueInDomain(18));
  UI_domain dc(3);
  bool answer_C[6]={false,false,false,true,false,false};
  std::cout << "domain C :"; for (unsigned int i=0; i<6; ++i) assert(dc.valueInDomain(i)==answer_C[i]);

  aboveFirstThreshold T1;
  aboveSecondThreshold T2;
  aboveThirdThreshold T3;
  bool answer_T[5]={false,true,true,true,false};
  for (unsigned int i=0; i<5; ++i) {std::cout << T1.valueInDomain(i) << " "; assert(T1.valueInDomain(i)==answer_T[i]);}  std::cout<<std::endl;
  answer_T[2]=false;
  for (unsigned int i=0; i<5; ++i) {std::cout << T2.valueInDomain(i) << " "; assert(T2.valueInDomain(i)==answer_T[i]);}  std::cout<<std::endl;
  answer_T[1]=false;
  for (unsigned int i=0; i<5; ++i) {std::cout << T3.valueInDomain(i) << " "; assert(T3.valueInDomain(i)==answer_T[i]);}  std::cout<<std::endl;

  return 0;
}
