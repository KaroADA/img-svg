/**
 * @file kmeans.hpp
 * @brief Reducer for image color depth using KMeans clustering.
 */

#pragma once

#include "types.hpp"

namespace Stage::KMeans {

/**
 * @brief Groups image pixels into a fixed number of color clusters.
 *
 * Randomly initializes cluster centers and iteratively updates them by 
 * assigning pixels to the nearest color and recalculating the mean color.
 *
 * @param image Input raw image data.
 * @param config Settings containing the target number of colors.
 * @return QuantizedImage Image data with pixels mapped to a color palette.
 */
QuantizedImage process(const Image& image, const Config& config);

}  // namespace Stage::KMeans
