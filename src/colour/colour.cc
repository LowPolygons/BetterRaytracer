#include "colour.hh"
#include "vectors/vector_definitions.hh"
#include <algorithm>
#include <cmath>
#include <vector>
using Colours::BasicColour;

auto Colours::get_average_of_colours(std::vector<BasicColour> colours)
    -> BasicColour {
  auto summed = BasicColour({0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0});

  for (auto colour : colours) {
    std::transform(colour.begin(), colour.end(), summed.begin(), summed.begin(),
                   [&](auto c, auto s) { return c + s; });
  }

  // Perform averaging, tone mapping and gamma correction:
  // -> For extremely bright objects  where brightness is like 10, tone mapping
  // allegedly preserves detail while neatly clamping values down
  // ===========
  // -> Doubling a channel != double the brightness, but this raytracer assumes
  // it does
  // -> Gamma correction is an standard method of correcting this

  auto constexpr gamma = 1.0 / 2.2f;

  std::transform(summed.begin(), summed.end(), summed.begin(), [&](auto sum) {
    auto avg = sum / colours.size();
    return std::pow((avg / (1.0f + avg)), gamma);
  });

  return summed;
}

auto Colours::ColourData::get_total_colour() -> BasicColour {
  return total_colour;
}

auto Colours::ColourData::combine_colour_as_average(BasicColour new_colour,
                                                    std::size_t bounce_info,
                                                    float contribution)
    -> void {
  previous_colours.push_back(new_colour);

  total_colour[3] += total_colour[0] * (new_colour[3] * new_colour[6]);
  total_colour[4] += total_colour[1] * (new_colour[4] * new_colour[6]);
  total_colour[5] += total_colour[2] * (new_colour[5] * new_colour[6]);

  total_colour[0] *= new_colour[0] * contribution;
  total_colour[1] *= new_colour[1] * contribution;
  total_colour[2] *= new_colour[2] * contribution;

  num_colours_accumulated++;
}
