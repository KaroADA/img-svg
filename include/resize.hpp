/**
 * @file resize.hpp
 * @brief Scales down images exceeding the maximum width.
 */

#pragma once

#include "types.hpp"

namespace Stage::Resize {

/**
 * @brief Proportionally resizes an image if it exceeds the maximum width.
 *
 * Modifies the image dimensions and pixel data in place. If the image
 * width is smaller than or equal to the maximum width, no changes are made.
 *
 * @param image The image data to be resized.
 * @param config Settings containing the maximum allowed width.
 */
void process(Image& image, const Config& config);

}  // namespace Stage::Resize
