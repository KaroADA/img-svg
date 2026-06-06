#include "edge_detection.hpp"
#include <catch2/catch_test_macros.hpp>
#include <vector>

TEST_CASE("EdgeDetection stage traces contours of connected regions",
          "[edge_detection]") {
  Config config{.output_path = "output.svg",
                .max_width = 0,
                .colors = 16,
                .min_area = 0,
                .tolerance = 1.0,
                .corner_threshold = 125.0,
                .smoothing = 1.3,
                .verbose = false};

  ImageRegions regions;
  regions.region_colors[1] = {255, 0, 0};
  regions.region_colors[2] = {0, 0, 255};

  SECTION("Case 1: Traces perimeter of a single uniform region") {
    regions.pixel_regions = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1},
    };

    auto result = Stage::EdgeDetection::process(regions, config);

    REQUIRE(result.size() == 1);
    REQUIRE(result[0].points.size() == 8);
    REQUIRE(result[0].color.r == 255);
  }

  SECTION("Case 2: Separates outer background and inner shape boundaries") {
    regions.pixel_regions = {
        {1, 1, 1},
        {1, 2, 1},
        {1, 1, 1},
    };

    auto result = Stage::EdgeDetection::process(regions, config);

    REQUIRE(result.size() == 2);

    bool found_inner = false;
    for (const auto& poly : result) {
      if (poly.color.b == 255) {
        found_inner = true;
        REQUIRE(poly.points.size() == 1);
        REQUIRE(poly.points[0].x == 1);
        REQUIRE(poly.points[0].y == 1);
      }
    }
    REQUIRE(found_inner == true);
  }
}
