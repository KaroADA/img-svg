#include "edge_detection.hpp"
#include <print>

namespace Stage::EdgeDetection {
std::vector<Polygon> process(const ImageRegions& regions,
                             const Config& config) {
  if (config.verbose) {
    std::println("Processing edge detection with parameters:");
    std::println("  min_area: {}", config.min_area);
    std::println("  tolerance: {}", config.tolerance);
  }

  std::vector<Polygon> all_polygons;

  // TODO: Implement edge detection and polygon approximation here.

  return all_polygons;
}
}  // namespace Stage::EdgeDetection
