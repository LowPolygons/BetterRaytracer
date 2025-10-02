// #include "colour.hh"
// #include <algorithm>
// #include <cmath>
// #include <vector>
// using Colours::BasicColour;
//
// auto Colours::get_average_of_colours(const std::vector<BasicColour> &colours,
//                                      const float &gamma) -> BasicColour {
//   auto summed = BasicColour({0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0});
//
//   // Sum the colours for a pixel
//   for (std::size_t c = 0; c < colours.size(); c++) {
//     for (std::size_t i = 0; i < summed.size(); i++)
//       summed[i] = summed[i] + colours[c][i];
//   }
//
//   // Average and colour-correct
//   for (std::size_t i = 0; i < summed.size(); i++) {
//     auto avg = summed[i] / colours.size();
//     summed[i] = std::pow((avg / (1.0f + avg)), gamma);
//   }
//
//   return summed;
// }
//
// auto Colours::ColourData::get_total_colour() -> const BasicColour & {
//   return total_colour;
// }
//
// // INFO: Couldn't figure it out for myself; taken from sebastian Lague's
// // raytracer!
// auto Colours::ColourData::combine_colour_as_average(
//     const BasicColour &new_colour, const std::size_t &bounce_info,
//     const float &contribution) -> void {
//   // First tint the previously accumulated light by the total colour received
//   // based on the new light
//   total_colour[3] =
//       total_colour[3] + total_colour[0] * (new_colour[3] * new_colour[6]);
//   total_colour[4] =
//       total_colour[4] + total_colour[1] * (new_colour[4] * new_colour[6]);
//   total_colour[5] =
//       total_colour[5] + total_colour[2] * (new_colour[5] * new_colour[6]);
//
//   // Then add the colour of the object scaled by the user-defined
//   constribution
//   // factor
//   total_colour[0] = total_colour[0] * new_colour[0] * contribution;
//   total_colour[1] = total_colour[1] * new_colour[1] * contribution;
//   total_colour[2] = total_colour[2] * new_colour[2] * contribution;
//
//   num_colours_accumulated++;
// }
