#ifndef COLOUR_COLOUR_DEFINITIONS

#define COLOUR_COLOUR_DEFINITIONS

#include "vectors/vector_definitions.hh"
#include <vector>

using Vectors::Vec;

namespace Colours {

// Colour: R, G, B, Emission: R, G, B, Strength, Specular: %
// Index:  0, 1, 2,           3, 4, 5         6,           7
using BasicColour = Vec<8, float>;

// A function used to do the final average of multiple arrays
auto get_average_of_colours(std::vector<BasicColour> colours) -> BasicColour;

// Each Ray has its own corresponding Colour Data
// TODO: Implement these methods
//
class ColourData {
private:
  BasicColour total_colour;

  std::vector<BasicColour> previous_colours;
  std::size_t num_colours_accumulated;

public:
  ColourData() {
    num_colours_accumulated = 0;
    total_colour = BasicColour{1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  };

  // Given a new colour, adds it to the list of previous_colours
  // and then joins it to the total colour
  auto combine_colour_as_average(BasicColour new_colour) -> void;

  auto get_total_colour() -> BasicColour;
};

} // namespace Colours

#endif
