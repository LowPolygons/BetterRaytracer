#include "raytracer.hh"
#include "colour/colour.hh"
#include "geometry/sphere.hh"
#include "geometry/triangle.hh"
#include "vectors/vector_definitions.hh"

using Colours::BasicColour;
using Vectors::Vec;

auto Raytracer::add_triangle(Triangle triangle) -> void {
  triangles.push_back(triangle);
}
auto Raytracer::add_sphere(Sphere sphere) -> void { spheres.push_back(sphere); }

auto Raytracer::add_cuboid(Vec<3, double> LDB, Vec<3, double> RDB,
                           Vec<3, double> RDF, Vec<3, double> LDF,
                           Vec<3, double> LUB, Vec<3, double> RUB,
                           Vec<3, double> RUF, Vec<3, double> LUF, //
                           BasicColour D1, BasicColour D2, BasicColour L1,
                           BasicColour L2, BasicColour R1, BasicColour R2,
                           BasicColour B1, BasicColour B2, BasicColour F1,
                           BasicColour F2, BasicColour U1, BasicColour U2)
    -> void {
  // Form the 12 triangles

  // Down
  add_triangle(Triangle(LDB, RDB, RDF, B1));
  add_triangle(Triangle(LDB, LDF, RDF, B2));

  // Up
  add_triangle(Triangle(LUB, RUB, RUF, U1));
  add_triangle(Triangle(LUB, LUF, RUF, U2));

  // Left
  add_triangle(Triangle(LDB, LUB, LUF, L1));
  add_triangle(Triangle(LDB, LDF, LUF, L2));

  // Right
  add_triangle(Triangle(RDB, RUB, RUF, R1));
  add_triangle(Triangle(RDB, RDF, RUF, R2));

  // Back
  add_triangle(Triangle(LDB, RDB, RUB, B1));
  add_triangle(Triangle(LDB, LUB, RUB, B2));

  // Forward
  add_triangle(Triangle(LDF, RDF, RUF, F1));
  add_triangle(Triangle(LDF, LUF, RUF, F2));
}
