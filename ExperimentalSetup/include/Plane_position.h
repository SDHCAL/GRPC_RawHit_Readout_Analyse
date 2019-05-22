#ifndef Plane_position_HH
#define Plane_position_HH

#include <math.h>
#include <array>

class Plane_position
{
 public:
  Plane_position() : position_origin({0,0,0}), direction_I({1,0,0}), direction_J({0,1,0}) {;}
  Plane_position(std::array<float,3> origin,std::array<float,3> dI,std::array<float,3> dJ) : position_origin(origin), direction_I(dI), direction_J(dJ) {normalize_directions();}
  void setPosition(std::array<float,3> d) {position_origin=d;}
  void setDirections(std::array<float,3> I,std::array<float,3> J) {direction_I=I;direction_J=J;normalize_directions();}
  void getPosition(float xI, float xJ, float* pos) const { for (int i=0; i<3; ++i) pos[i]=position_origin[i]+xI*direction_I[i]+xJ*direction_J[i];}
  void getPosition(float xI, float xJ, std::array<float,3>& pos) const { getPosition(xI,xJ,pos.data());}
 private:
  std::array<float,3> position_origin;
  std::array<float,3> direction_I;
  std::array<float,3> direction_J;
  double norme(std::array<float,3>& vec) {return sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]);}
  void normalize_directions();
};

#endif
