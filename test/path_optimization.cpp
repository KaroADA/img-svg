#include "path_optimization.hpp"
#include <catch2/catch_test_macros.hpp>
#include <vector>

TEST_CASE("PathOptimization stage simplifies polygons using RDP algorithm",
          "[path_optimization]") {
  Config config{.output_path = "output.svg",
                .max_width = 0,
                .colors = 16,
                .min_area = 10,
                .tolerance = 1.0,
                .corner_threshold = 125.0,
                .smoothing = 1.3,
                .verbose = false};

  SECTION("Case 1: Collinear points removal on a straight line") {
    Polygon poly;
    poly.color = {255, 0, 0};
    poly.points = {{0, 0}, {5, 0}, {10, 0}};

    std::vector<Polygon> input = {poly};
    config.tolerance = 0.1;

    auto result = Stage::PathOptimization::process(input, config);

    REQUIRE(result.size() == 1);
    REQUIRE(result[0].points.size() == 2);
    REQUIRE(result[0].points[0].x == 0);
    REQUIRE(result[0].points[1].x == 10);
  }

  SECTION("Case 2: Point removal within deviation tolerance") {
    Polygon poly;
    poly.points = {{0, 0}, {5, 1}, {10, 0}};
    std::vector<Polygon> input = {poly};

    config.tolerance = 2.0;
    auto result = Stage::PathOptimization::process(input, config);

    REQUIRE(result.size() == 1);
    REQUIRE(result[0].points.size() == 2);
    REQUIRE(result[0].points[0].x == 0);
    REQUIRE(result[0].points[1].x == 10);
  }

  SECTION("Case 3: Point preservation outside deviation tolerance") {
    Polygon poly;
    poly.points = {{0, 0}, {5, 5}, {10, 0}};
    std::vector<Polygon> input = {poly};

    config.tolerance = 2.0;
    auto result = Stage::PathOptimization::process(input, config);

    REQUIRE(result.size() == 1);
    REQUIRE(result[0].points.size() == 3);
    REQUIRE(result[0].points[1].x == 5);
    REQUIRE(result[0].points[1].y == 5);
  }

  SECTION("Case 4: Handling of polygons with 2 points") {
    Polygon poly;
    poly.points = {{0, 0}, {10, 10}};
    std::vector<Polygon> input = {poly};

    config.tolerance = 5.0;
    auto result = Stage::PathOptimization::process(input, config);

    REQUIRE(result.size() == 1);
    REQUIRE(result[0].points.size() == 2);
  }
}
