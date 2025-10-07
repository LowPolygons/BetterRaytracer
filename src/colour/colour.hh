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
constexpr auto get_average_of_colours(const std::vector<BasicColour> &colours,
                                      const float &gamma) -> BasicColour;
constexpr auto get_average_of_colours(BasicColour &colours,
                                      const std::size_t &num_aggregated,
                                      const float &gamma) -> void;

// Each ray will have a ColourData object associated with it
class ColourData {
private:
  BasicColour total_colour;

  std::size_t num_colours_accumulated;

public:
  ColourData() {
    num_colours_accumulated = 0;
    total_colour = BasicColour{1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  };

  // Given a new colour, adds it to the list of previous_colours
  // and then joins it to the total colour
  constexpr auto combine_colour_as_average(const BasicColour &new_colour,
                                           const std::size_t &bounce_info,
                                           const float &contribution) -> void;

  constexpr auto get_total_colour() -> const BasicColour &;
};

} // namespace Colours

#include "colour.hh"
#include <algorithm>
#include <cmath>
#include <vector>
using Colours::BasicColour;

constexpr auto
Colours::get_average_of_colours(BasicColour &summed,
                                const std::size_t &num_aggregated,
                                const float &gamma) -> void {
  // Average and colour-correct
  for (std::size_t i = 0; i < summed.size(); i++) {
    auto avg = summed[i] / num_aggregated;
    summed[i] = std::pow((avg / (1.0f + avg)), gamma);
  }
}

constexpr auto
Colours::get_average_of_colours(const std::vector<BasicColour> &colours,
                                const float &gamma) -> BasicColour {
  auto summed = BasicColour({0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0});

  // Sum the colours for a pixel
  for (std::size_t c = 0; c < colours.size(); c++) {
    for (std::size_t i = 0; i < summed.size(); i++)
      summed[i] = summed[i] + colours[c][i];
  }

  // Average and colour-correct
  for (std::size_t i = 0; i < summed.size(); i++) {
    auto avg = summed[i] / colours.size();
    summed[i] = std::pow((avg / (1.0f + avg)), gamma);
  }

  return summed;
}

constexpr auto Colours::ColourData::get_total_colour() -> const BasicColour & {
  return total_colour;
}

// INFO: Couldn't figure it out for myself; taken from sebastian Lague's
// raytracer!
constexpr auto Colours::ColourData::combine_colour_as_average(
    const BasicColour &new_colour, const std::size_t &bounce_info,
    const float &contribution) -> void {
  // First tint the previously accumulated light by the total colour received
  // based on the new light
  total_colour[3] =
      total_colour[3] + total_colour[0] * (new_colour[3] * new_colour[6]);
  total_colour[4] =
      total_colour[4] + total_colour[1] * (new_colour[4] * new_colour[6]);
  total_colour[5] =
      total_colour[5] + total_colour[2] * (new_colour[5] * new_colour[6]);

  // Then add the colour of the object scaled by the user-defined constribution
  // factor
  total_colour[0] = total_colour[0] * new_colour[0] * contribution;
  total_colour[1] = total_colour[1] * new_colour[1] * contribution;
  total_colour[2] = total_colour[2] * new_colour[2] * contribution;

  num_colours_accumulated++;
}

#endif
