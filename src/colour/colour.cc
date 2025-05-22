#include "colour.hh"
#include "vectors/vector_definitions.hh"
#include <vector>

#include <algorithm>
using Colours::BasicColour;

auto Colours::get_average_of_colours(std::vector<BasicColour> colours)
    -> BasicColour {
  auto summed = BasicColour({0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0});

  for (auto colour : colours) {
    std::transform(colour.begin(), colour.end(), summed.begin(), summed.begin(),
                   [&](auto c, auto s) { return c + s; });
  }

  std::transform(summed.begin(), summed.end(), summed.begin(),
                 [&](auto sum) { return sum / colours.size(); });

  return summed;
}

auto Colours::ColourData::get_total_colour() -> BasicColour {
  return total_colour;
}

auto Colours::ColourData::combine_colour_as_average(BasicColour new_colour)
    -> void {
  previous_colours.push_back(new_colour);

  total_colour[3] += total_colour[0] * (new_colour[3] * new_colour[6]);
  total_colour[4] += total_colour[1] * (new_colour[4] * new_colour[6]);
  total_colour[5] += total_colour[2] * (new_colour[5] * new_colour[6]);

  total_colour[0] *= new_colour[0];
  total_colour[1] *= new_colour[1];
  total_colour[2] *= new_colour[2];

  num_colours_accumulated++;
}
