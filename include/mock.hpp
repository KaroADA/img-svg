/**
 * @file mock.hpp
 * @brief Mock functions for testing segmentation logic.
 */

#include "types.hpp"

/**
 * @brief Splits an image into two horizontal halves.
 *
 * @param img Input image to split.
 * @return ImageRegions Two regions (top and bottom) with red and blue colors.
 */
ImageRegions mock_segmentation(const Image& img);

/**
 * @brief Creates a circular and a rectangular region in the image.
 *
 * @param img Input image.
 * @return ImageRegions Regions representing a background, a circle and a box.
 */
ImageRegions mock_segmentation_geom(const Image& img);
