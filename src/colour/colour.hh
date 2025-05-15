#ifndef COLOUR_COLOUR_DEFINITIONS

#define COLOUR_COLOUR_DEFINITIONS

#include "vectors/vector_definitions.hh"
#include <vector>

using Vectors::Vec;

namespace Colours {

// Colour: R, G, B, Emission: R, G, B, Strength, Specular: %
// Index:  0, 1, 2,           3, 4, 5         6,           7
using BasicColour = Vec<8, float>;

class ColourData {
private:
  BasicColour total_colour;

  std::vector<BasicColour> previous_colours;
  std::size_t num_colours_accumulated;

public:
  ColourData(BasicColour _colour) : total_colour(_colour) {
    num_colours_accumulated = 1;
  };

  // Given a new colour, adds it to the list of previous_colours
  // and the joins it to the total colour
  auto combine_colour_as_average() -> void;

  // TODO: add any other needed methods for colour handling
};

} // namespace Colours

#endif
