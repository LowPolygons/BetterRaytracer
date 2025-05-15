#ifndef RAYLOGIC_RAYLOGIC_METHODS_HH

#define RAYLOGIC_RAYLOGIC_METHODS_HH

#include "colour/colour.hh"
#include "vectors/vector_definitions.hh"
#include "vectors/vector_methods.hh"

using Colours::BasicColour;
using Vectors::Line;

namespace RayLogic {
auto calculate_new_ray_direction(const Line<3, double> &ray,
                                 const Vec<3, double> &p_of_i,
                                 const Vec<3, double> &normal,
                                 const BasicColour &object_colour)
    -> Line<3, double>;
}

#endif
