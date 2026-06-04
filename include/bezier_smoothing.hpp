/**
 * @file bezier_smoothing.hpp
 * @brief Converts polygon edges into smooth Bezier curves.
 */

#pragma once

#include <vector>
#include "types.hpp"

namespace Stage::BezierSmoothing {

/**
 * @brief Converts polygons into smooth vector shapes.
 *
 * Calculates the angle at each polygon point. If the angle is larger than
 * the corner threshold, it creates a smooth curve. If the angle is smaller,
 * it keeps a sharp corner.
 *
 * @param polygons A list of input polygons.
 * @param config Settings for smoothing, like corner threshold and optimize factor.
 * @return std::vector<Shape> List of shapes made of Bezier curves and straight lines.
 */
std::vector<Shape> process(const std::vector<Polygon>& polygons,
                           const Config& config);

}  // namespace Stage::BezierSmoothing
