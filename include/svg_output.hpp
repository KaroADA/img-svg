/**
 * @file svg_output.hpp
 * @brief Exports vector shapes to an SVG file.
 */

#pragma once

#include <vector>
#include "types.hpp"

namespace Stage::SvgOutput {

/**
 * @brief Writes shapes into a formatted SVG file.
 *
 * Converts internal shape representations into SVG path elements.
 *
 * @param shapes List of Bezier curves and lines to export.
 * @param width Canvas width.
 * @param height Canvas height.
 * @param config Settings containing the output file path.
 */
void process(const std::vector<Shape>& shapes, int width, int height,
             const Config& config);

}  // namespace Stage::SvgOutput
