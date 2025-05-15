#ifndef RAYLOGIC_RAYLOGIC_METHODS_HH

#define RAYLOGIC_RAYLOGIC_METHODS_HH

#include "vectors/vector_definitions.hh"
#include "vectors/vector_methods.hh"

using Vectors::Line;

namespace RayLogic {
auto calculate_new_ray_direction(const Line<3, double> &ray,
                                 const Line<3, double> &normal,
                                 const float &specularity) -> Line<3, double>;
}

#endif
