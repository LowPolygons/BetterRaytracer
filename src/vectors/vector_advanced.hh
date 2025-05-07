#include "vector_definitions.hh"

#include <cmath>
#include <cstddef>
#include <optional>
#include <utility>

namespace Vectors {

enum AngleReturnOption {
  ACUTE,
  OBTUSE,
};

// Alias for the determined type of two other types added together
template <typename T1, typename T2>
using JointType = decltype(std::declval<T1>() + std::declval<T2>());

// Calculates the normal of a plane
template <typename P>
auto calculate_normal_of_plane(Plane<P> &plane) -> Vec<3, P>;

// Does a given point lie on a plane of any type
template <typename V, typename P>
auto is_point_on_plane(Vec<3, V> &point, Plane<P> &plane) -> bool;

// Returns the point of intersection and the lambda value, or nothing
template <typename V, typename P>
auto constexpr line_intersects_plane(Line<3, V> &line, Plane<P> &plane)
    -> std::optional<std::pair<JointType<V, P>, Vec<3, JointType<V, P>>>>;

// Returns the shortest distance between a line and a point
template <std::size_t Vs, typename V1, typename V2>
auto constexpr point_to_line_distance(Line<Vs, V1> &line, Vec<Vs, V2> &point)
    -> Vec<Vs, JointType<V1, V2>>;

// Returns the acute or obstuse angle between a line and a plane, or nothing
template <AngleReturnOption ARO, typename V, typename P>
auto constexpr angle_between_line_and_plane(Line<3, V> &line, Plane<P> &plane)
    -> std::optional<JointType<V, P>>;

// Reflects a point across a plane
template <typename V, typename P>
auto constexpr reflect_point_across_plane(Vec<3, V> &point, Plane<P> &plane)
    -> Vec<3, JointType<V, P>>;

} // namespace Vectors
