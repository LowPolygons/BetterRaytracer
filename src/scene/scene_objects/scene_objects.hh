#ifndef WINDOW_SCENEOBJECTS_SCENEOBJECTS_HH
#define WINDOW_SCENEOBJECTS_SCENEOBJECTS_HH

#include "colour/colour.hh"
#include "geometry/shape.hh"
#include "geometry/sphere.hh"
#include "geometry/triangle.hh"
#include "vectors/vector_definitions.hh"
#include <memory>

using Colours::BasicColour;
using Vectors::Vec;

// Container of geometry used by the raytracer
class SceneObjects {
private:
  // std::vector<Geometry> shapes;
  std::vector<std::shared_ptr<Shape>> shapes;

public:
  SceneObjects() {};

  auto add_triangle(const Triangle &triangle) -> void;
  auto add_sphere(const Sphere &sphere) -> void;
  // Naming convention for each coordinate is: X, Y, Z
  // LR, DU, BF = left/right, down/up backward/forward
  auto add_cuboid(Vec<3, double> LDB, Vec<3, double> RDB, Vec<3, double> RDF,
                  Vec<3, double> LDF, Vec<3, double> LUB, Vec<3, double> RUB,
                  Vec<3, double> RUF, Vec<3, double> LUF, //
                  BasicColour D1, BasicColour D2, BasicColour L1,
                  BasicColour L2, BasicColour R1, BasicColour R2,
                  BasicColour B1, BasicColour B2, BasicColour F1,
                  BasicColour F2, BasicColour U1, BasicColour U2) -> void;

  // auto get_shapes() -> std::vector<Geometry> &;
  auto get_shapes() const -> const std::vector<std::shared_ptr<Shape>> &;
};

#endif
