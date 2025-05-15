#ifndef GEOMETRY_SHAPES_TRIANGLE_HH

#define GEOMETRY_SHAPES_TRIANGLE_HH

#include "colour/colour.hh"
#include "shape.hh"
#include "vectors/vector_definitions.hh"
#include "vectors/vector_methods.hh"
#include <cstddef>

using Colours::BasicColour;
using Vectors::Line;
using Vectors::Plane;
using Vectors::Vec;

class Triangle {
public:
  // A function which picks which two vectors should be used for barycentrics
  auto get_valid_vectors() -> void;

  auto check_intersection(Line<3, double> ray) const -> bool;

  // Getters
  auto get_corners() -> std::array<Vec<3, double>, 3> { return corners; }
  auto get_vectors() -> std::array<Vec<3, double>, 2> { return vectors; }

  Triangle(              //
      Vec<3, double> v1, //
      Vec<3, double> v2, //
      Vec<3, double> v3, //
      BasicColour clr    //
  ) {
    corners = {v1, v2, v3};
    get_valid_vectors();
    colour_properties = clr;

    // TODO: rewrite when function calls accept const lvalues
    auto vec1 = vectors.at(0);
    auto vec2 = vectors.at(1);
    auto point = corners.at(1);

    // Form the planes normal and dval
    auto normal = Vectors::cross(vec1, vec2);
    auto dval = Vectors::dot(normal, point);

    triangle_plane = std::make_pair(normal, dval);
  };

private:
  std::array<Vec<3, double>, 3> corners;
  // Two vectors with a common point
  std::array<Vec<3, double>, 2> vectors;
  // Indexes of coefs to be used in the simultaneous EQs to get barycentric vals
  Vec<2, std::size_t> indexs_for_baryc;
  // Index of the corner which the vectors subtend from
  std::size_t index_of_vec_origin;
  // Plane the triangle lies in
  Plane<double> triangle_plane;

  BasicColour colour_properties;
};

#endif
