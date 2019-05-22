#include "Plane_position.h"

void Plane_position::normalize_directions()
{
  float normeI=norme(direction_I);
  float normeJ=norme(direction_J);
  for (int i=0; i<3; ++i)
    {
      direction_I[i]/=normeI;
      direction_J[i]/=normeJ;
    }
}
