/**
 * @file edge_detection.hpp
 * @brief Extracts polygons from image regions.
 */

#pragma once

#include "types.hpp"

namespace Stage::EdgeDetection {

/**
 * @brief Traces region boundaries to create polygons.
 *
 * Iterates through pixels to find edges between different regions.
 * Follows the perimeter of each region and stores it as a list of points.
 *
 * @param regions Input grid of region IDs and their colors.
 * @param config Settings containing minimum area filter.
 * @return std::vector<Polygon> List of polygons representing region outlines.
 */
std::vector<Polygon> process(const ImageRegions& regions, const Config& config);

}  // namespace Stage::EdgeDetection
