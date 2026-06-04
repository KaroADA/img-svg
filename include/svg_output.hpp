#pragma once
#include <vector>
#include "types.hpp"

namespace Stage::SvgOutput {
void process(const std::vector<Shape>& shapes, int width, int height,
             const Config& config);
}  // namespace Stage::SvgOutput
