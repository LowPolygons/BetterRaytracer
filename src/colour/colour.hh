#ifndef COLOUR_COLOUR_DEFINITIONS

#define COLOUR_COLOUR_DEFINITIONS

#include "vectors/vector_definitions.hh"
#include <vector>

using Vectors::Vec;

namespace Colours {

// For potentially large scenes with more objects, having the colour container
// be an alias and the colour methods be stored in a separate class could be
// more wise
// Indexes:
// Colour: R(0), G(1), B(2) Emission: R(3), G(4), B(5) Strength(6) Gloss: %(7)
using BasicColour = Vec<8, float>;

// A function used to do the final average of multiple arrays
auto get_average_of_colours(std::vector<BasicColour> colours) -> BasicColour;

// Each ray will have a ColourData object associated with it
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
  auto combine_colour_as_average(BasicColour new_colour,
                                 std::size_t bounce_info, float contribution)
      -> void;

  auto get_total_colour() -> const BasicColour &;
};

} // namespace Colours

#endif
