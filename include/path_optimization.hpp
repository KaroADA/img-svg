/**
 * @file path_optimization.hpp
 * @brief Simplifies polygons by removing redundant points.
 */

#pragma once

#include <vector>
#include "types.hpp"

namespace Stage::PathOptimization {

/**
 * @brief Reduces the number of points in polygons.
 *
 * Uses the RDP algorithm to remove points that deviate
 * less than the specified tolerance from a straight line.
 *
 * @param polygons List of input polygons.
 * @param config Settings containing the distance tolerance.
 * @return std::vector<Polygon> Simplified polygons with fewer vertices.
 */
std::vector<Polygon> process(const std::vector<Polygon>& polygons,
                             const Config& config);

}  // namespace Stage::PathOptimization
