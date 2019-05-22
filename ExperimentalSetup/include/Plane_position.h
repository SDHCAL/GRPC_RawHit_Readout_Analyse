#ifndef Plane_position_HH
#define Plane_position_HH

#include <math.h>
#include <array>

class Plane_position
{
 public:
  Plane_position() {;}
 private:
  std::array<double,3> position_origin;
  std::array<double,3> direction_I;
  std::array<double,3> direction_J;
  double norme(std::array<double,3>& vec) {return sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]);}
  void normalize_directions();
};

#endif
