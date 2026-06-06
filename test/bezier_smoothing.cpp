#include "bezier_smoothing.hpp"
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <vector>

using Catch::Approx;

TEST_CASE("BezierSmoothing stage calculates curves based on corner angles",
          "[bezier_smoothing]") {
  Config config{.output_path = "output.svg",
                .max_width = 0,
                .colors = 16,
                .min_area = 10,
                .tolerance = 1.0,
                .corner_threshold = 125.0,
                .smoothing = 1.0,
                .stroke_width = 1.0,
                .verbose = false};

  SECTION("Case 1: Keeps sharp corners for right angles") {
    Polygon poly;
    poly.points = {{0, 0}, {10, 0}, {10, 10}, {0, 10}};
    std::vector<Polygon> input = {poly};

    auto result = Stage::BezierSmoothing::process(input, config);

    REQUIRE(result.size() == 1);
    REQUIRE(result[0].nodes.size() == 4);
    for (const auto& node : result[0].nodes) {
      REQUIRE(node.is_curve == false);
    }
  }

  SECTION("Case 2: Calculation of Bezier control points for wide angles") {
    Polygon poly;
    poly.points = {{0, 0}, {5, 2}, {10, 0}};
    std::vector<Polygon> input = {poly};

    auto result = Stage::BezierSmoothing::process(input, config);

    REQUIRE(result.size() == 1);
    REQUIRE(result[0].nodes.size() == 3);
    REQUIRE(result[0].nodes[1].is_curve == true);

    double forward_cx = 5.0 + (std::sqrt(29.0) * 0.25);
    double backward_cx = 5.0 - (std::sqrt(29.0) * 0.25);

    REQUIRE(result[0].nodes[1].control2.x ==
            Approx(backward_cx).epsilon(0.001));
    REQUIRE(result[0].nodes[1].control2.y == Approx(2.0).epsilon(0.001));

    REQUIRE(result[0].nodes[2].is_curve == true);
    REQUIRE(result[0].nodes[2].control1.x == Approx(forward_cx).epsilon(0.001));
    REQUIRE(result[0].nodes[2].control1.y == Approx(2.0).epsilon(0.001));
  }

  SECTION(
      "Case 3: Passes polygons with fewer than 3 points as straight lines") {
    Polygon poly;
    poly.points = {{0, 0}, {10, 10}};
    std::vector<Polygon> input = {poly};

    auto result = Stage::BezierSmoothing::process(input, config);

    REQUIRE(result.size() == 1);
    REQUIRE(result[0].nodes.size() == 2);
    REQUIRE(result[0].nodes[0].is_curve == false);
    REQUIRE(result[0].nodes[1].is_curve == false);
  }
}
