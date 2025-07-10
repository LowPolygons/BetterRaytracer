#include "colour.hh"
#include <algorithm>
#include <cmath>
#include <vector>
using Colours::BasicColour;

auto Colours::get_average_of_colours(const std::vector<BasicColour> &colours,
                                     const float &gamma) -> BasicColour {
  auto summed = BasicColour({0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0});

  // Sum the colours for a pixel
  for (auto colour : colours) {
    std::transform(colour.begin(), colour.end(), summed.begin(), summed.begin(),
                   [&](auto c, auto s) { return c + s; });
  }

  // Average and colour-correct
  std::transform(summed.begin(), summed.end(), summed.begin(), [&](auto sum) {
    auto avg = sum / colours.size();
    return std::pow((avg / (1.0f + avg)), gamma);
  });

  return summed;
}

auto Colours::ColourData::get_total_colour() -> const BasicColour & {
  return total_colour;
}

// INFO: Couldn't figure it out for myself; taken from sebastian Lague's
// raytracer!
auto Colours::ColourData::combine_colour_as_average(
    const BasicColour &new_colour, const std::size_t &bounce_info,
    const float &contribution) -> void {
  previous_colours.push_back(new_colour);

  // First tint the previously accumulated light by the total colour received
  // based on the new light
  total_colour[3] += total_colour[0] * (new_colour[3] * new_colour[6]);
  total_colour[4] += total_colour[1] * (new_colour[4] * new_colour[6]);
  total_colour[5] += total_colour[2] * (new_colour[5] * new_colour[6]);

  // Then add the colour of the object scaled by the user-defined constribution
  // factor
  total_colour[0] *= new_colour[0] * contribution;
  total_colour[1] *= new_colour[1] * contribution;
  total_colour[2] *= new_colour[2] * contribution;

  num_colours_accumulated++;
}
