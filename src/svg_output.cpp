#include "svg_output.hpp"
#include <format>
#include <fstream>
#include <iostream>
#include <print>

namespace Stage::SvgOutput {

void process(const std::vector<Polygon>& polygons, int width, int height,
             const Config& config) {
  if (config.verbose) {
    std::println("Exporting {} polygons to SVG: {}", polygons.size(),
                 config.output_path);
  }

  std::ofstream file(config.output_path);
  if (!file.is_open()) {
    std::println(std::cerr,
                 "Error: Could not open output file '{}' for writing.",
                 config.output_path);
    return;
  }

  std::println(file,
               "<svg width=\"{}\" height=\"{}\" viewBox=\"0 0 {} {}\" "
               "xmlns=\"http://www.w3.org/2000/svg\">",
               width, height, width, height);

  for (const auto& poly : polygons) {
    if (poly.points.empty())
      continue;

    std::string points_str;
    for (const auto& pt : poly.points) {
      points_str += std::format("{},{} ", pt.x, pt.y);
    }

    if (!points_str.empty()) {
      points_str.pop_back();
    }

    std::println(file,
                 "  <polygon points=\"{}\" fill=\"#{:02x}{:02x}{:02x}\" />",
                 points_str, poly.color.r, poly.color.g, poly.color.b);
  }

  std::println(file, "</svg>");

  if (config.verbose) {
    std::println("SVG export completed successfully.");
  }
}

}  // namespace Stage::SvgOutput
