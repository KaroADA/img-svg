#include "svg_output.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>

TEST_CASE("SvgOutput stage writes vector data to a valid SVG file",
          "[svg_output]") {
  Config config{.output_path = "test_output.svg",
                .max_width = 0,
                .colors = 16,
                .min_area = 10,
                .tolerance = 1.0,
                .corner_threshold = 125.0,
                .smoothing = 1.3,
                .stroke_width = 1.0,
                .verbose = false};

  SECTION("Case 1: Exports straight lines and curves to SVG format") {
    Shape shape;
    shape.color = {255, 0, 0};

    shape.nodes.push_back(PathNode{.position = {0.0, 0.0}, .is_curve = false});
    shape.nodes.push_back(PathNode{.position = {10.0, 0.0}, .is_curve = false});
    shape.nodes.push_back(PathNode{.position = {10.0, 10.0},
                                   .control1 = {10.0, 5.0},
                                   .control2 = {5.0, 10.0},
                                   .is_curve = true});

    std::vector<Shape> shapes = {shape};

    Stage::SvgOutput::process(shapes, 100, 100, config);

    std::ifstream file(config.output_path);
    REQUIRE(file.is_open() == true);

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();

    REQUIRE_THAT(content, Catch::Matchers::ContainsSubstring("<svg"));
    REQUIRE_THAT(content, Catch::Matchers::ContainsSubstring(
                              "fill=\"#ff0000\""));  // Hex color check
    REQUIRE_THAT(content,
                 Catch::Matchers::ContainsSubstring("M 0.00 0.00"));  // Move to
    REQUIRE_THAT(content,
                 Catch::Matchers::ContainsSubstring("L 10.00 0.00"));  // Line
    REQUIRE_THAT(content, Catch::Matchers::ContainsSubstring(
                              "C 10.00 5.00"));  // Curve control

    std::remove(config.output_path.c_str());
  }
}
