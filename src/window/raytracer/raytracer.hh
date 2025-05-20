#ifndef WINDOW_RAYTRACER_HH
#define WINDOW_RAYTRACER_HH

#include "geometry/sphere.hh"
#include "geometry/triangle.hh"
#include "vectors/vector_definitions.hh"

using Vectors::Vec;

// Raytracer only needs one thing from the screen and it can be copied by value
// therefore worthwhile storing as a separate object
class Raytracer {
private:
  std::vector<Sphere> spheres;
  std::vector<Triangle> triangles;

  // PIXEL DATA
public:
  Raytracer() {};

  auto add_triangle(Triangle triangle) -> void;
  auto add_sphere(Sphere sphere) -> void;
  // Naming convention for each coordinate is: X, Y, Z
  // LR, DU, BF = left/right, down/up backward/forward
  auto add_cuboid(Vec<3, double> LDB, Vec<3, double> RDB, Vec<3, double> RDF,
                  Vec<3, double> LDF, Vec<3, double> LUB, Vec<3, double> RUB,
                  Vec<3, double> RUF, Vec<3, double> LUF) -> void;
};

#endif
