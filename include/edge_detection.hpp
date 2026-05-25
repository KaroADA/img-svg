#pragma once
#include "types.hpp"

namespace Stage::EdgeDetection {
std::vector<Polygon> process(const ImageRegions& regions, const Config& config);
}  // namespace Stage::EdgeDetection
