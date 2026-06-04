/**
 * @file union_find.hpp
 * @brief Groups pixels into connected regions and filters noise.
 */

#pragma once

#include "types.hpp"

namespace Stage::UnionFind {

/**
 * @brief Identifies connected components of the same color.
 *
 * Groups adjacent pixels with the same label into regions. 
 * Small regions are merged with their largest neighbors to reduce noise.
 *
 * @param image Quantized image with pixel labels.
 * @param config Settings containing the minimum area threshold.
 * @return ImageRegions Data structure containing the identified regions and colors.
 */
ImageRegions process(const QuantizedImage& image, const Config& config);

}  // namespace Stage::UnionFind
