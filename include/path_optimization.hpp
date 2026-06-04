#pragma once

#include <vector>
#include "types.hpp"

namespace Stage::PathOptimization {

std::vector<Polygon> process(const std::vector<Polygon>& polygons,
                             const Config& config);

}  // namespace Stage::PathOptimization
