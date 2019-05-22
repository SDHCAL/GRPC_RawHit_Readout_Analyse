#ifndef Plane_position_HH
#define Plane_position_HH

#include <math.h>
#include <array>

class Plane_position
{
 public:
  Plane_position() : position_origin({0,0,0}), direction_I({1,0,0}), direction_J({0,1,0}) {;}
  void setPosition(std::array<float,3> d) {position_origin=d;}
  void setDirections(std::array<float,3> I,std::array<float,3> J) {direction_I=I;direction_J=J;normalize_directions();}
  //void getPosition(float xI, float yI, float* pos)
 private:
  std::array<float,3> position_origin;
  std::array<float,3> direction_I;
  std::array<float,3> direction_J;
  double norme(std::array<float,3>& vec) {return sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]);}
  void normalize_directions();
};

#endif
