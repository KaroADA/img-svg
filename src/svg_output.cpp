#include "svg_output.hpp"
#include <format>
#include <fstream>
#include <iostream>
#include <print>

namespace Stage::SvgOutput {

void process(const std::vector<Shape>& shapes, int width, int height,
             const Config& config) {
  if (config.verbose) {
    std::println("Exporting {} shapes to SVG: {}", shapes.size(),
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

  for (const auto& shape : shapes) {
    if (shape.nodes.empty())
      continue;

    std::string d_path;
    d_path += std::format("M {:.2f} {:.2f} ", shape.nodes[0].position.x,
                          shape.nodes[0].position.y);

    const int n = shape.nodes.size();

    for (int i = 1; i <= n; ++i) {
      const auto& node = shape.nodes[i % n];

      if (node.is_curve) {
        d_path +=
            std::format("C {:.2f} {:.2f}, {:.2f} {:.2f}, {:.2f} {:.2f} ",
                        node.control1.x, node.control1.y, node.control2.x,
                        node.control2.y, node.position.x, node.position.y);
      } else {
        d_path +=
            std::format("L {:.2f} {:.2f} ", node.position.x, node.position.y);
      }
    }
    d_path += "Z";

    if (config.stroke_width > 0.0) {
      std::println(file,
                   "  <path d=\"{}\" fill=\"#{:02x}{:02x}{:02x}\" "
                   "stroke=\"#{:02x}{:02x}{:02x}\" stroke-width=\"{:.2f}\" "
                   "stroke-linejoin=\"round\" />",
                   d_path, shape.color.r, shape.color.g, shape.color.b,
                   shape.color.r, shape.color.g, shape.color.b,
                   config.stroke_width);
    } else {
      std::println(file, "  <path d=\"{}\" fill=\"#{:02x}{:02x}{:02x}\" />",
                   d_path, shape.color.r, shape.color.g, shape.color.b);
    }
  }

  std::println(file, "</svg>");

  if (config.verbose) {
    std::println("SVG export completed successfully.");
  }
}

}  // namespace Stage::SvgOutput
