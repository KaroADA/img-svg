#pragma once

#include <vector>
#include "types.hpp"

namespace Stage::BezierSmoothing {

std::vector<Shape> process(const std::vector<Polygon>& polygons,
                           const Config& config);

}  // namespace Stage::BezierSmoothing
