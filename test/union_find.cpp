#include "union_find.hpp"
#include <catch2/catch_test_macros.hpp>
#include <vector>

namespace {
enum ColorLabel {
  RED_LABEL = 0,
  GREEN_LABEL = 1,
  BLUE_LABEL = 2,
  WHITE_LABEL = 3
};

const std::vector<Color> TEST_PALETTE = {
    Color{255, 0, 0}, Color{0, 255, 0}, Color{0, 0, 255}, Color{255, 255, 255}};
}  // namespace

TEST_CASE("UnionFind stage groups pixels and eliminates noise",
          "[union_find]") {
  Config config{.output_path = "output.svg",
                .max_width = 0,
                .colors = 4,
                .min_area = 0,
                .tolerance = 1.0,
                .corner_threshold = 125.0,
                .optimize = 1.3,
                .verbose = false};

  SECTION("Core connected components with noise reduction disabled") {
    config.min_area = 0;

    SECTION("Case 1: Standard contiguous grouping") {
      QuantizedImage qimg{.w = 3,
                          .h = 3,
                          .pixel_labels = {RED_LABEL, RED_LABEL, BLUE_LABEL,
                                           RED_LABEL, RED_LABEL, BLUE_LABEL,
                                           BLUE_LABEL, BLUE_LABEL, BLUE_LABEL},
                          .palette = TEST_PALETTE};

      ImageRegions result = Stage::UnionFind::process(qimg, config);

      REQUIRE(result.region_colors.size() == 2);

      int reg0 = result.pixel_regions[0][0];
      int reg1 = result.pixel_regions[0][2];

      REQUIRE(reg0 != reg1);

      REQUIRE(result.pixel_regions[0][1] == reg0);
      REQUIRE(result.pixel_regions[1][0] == reg0);
      REQUIRE(result.pixel_regions[1][1] == reg0);

      REQUIRE(result.pixel_regions[1][2] == reg1);
      REQUIRE(result.pixel_regions[2][0] == reg1);
      REQUIRE(result.pixel_regions[2][1] == reg1);
      REQUIRE(result.pixel_regions[2][2] == reg1);

      REQUIRE(result.region_colors[reg0].r == 255);
      REQUIRE(result.region_colors[reg1].b == 255);
    }

    SECTION("Case 2: Separation of non-adjacent regions of the same color") {
      QuantizedImage qimg{.w = 3,
                          .h = 3,
                          .pixel_labels = {RED_LABEL, BLUE_LABEL, RED_LABEL,
                                           BLUE_LABEL, BLUE_LABEL, BLUE_LABEL,
                                           RED_LABEL, BLUE_LABEL, RED_LABEL},
                          .palette = TEST_PALETTE};

      ImageRegions result = Stage::UnionFind::process(qimg, config);

      REQUIRE(result.region_colors.size() == 5);

      int tl = result.pixel_regions[0][0];
      int tr = result.pixel_regions[0][2];
      int bl = result.pixel_regions[2][0];
      int br = result.pixel_regions[2][2];
      int center = result.pixel_regions[1][1];

      REQUIRE(tl != tr);
      REQUIRE(tl != bl);
      REQUIRE(tl != br);
      REQUIRE(tl != center);
      REQUIRE(tr != bl);
      REQUIRE(tr != br);
      REQUIRE(tr != center);
      REQUIRE(bl != br);
      REQUIRE(bl != center);
      REQUIRE(br != center);

      REQUIRE(result.region_colors[tl].r == 255);
      REQUIRE(result.region_colors[tr].r == 255);
      REQUIRE(result.region_colors[bl].r == 255);
      REQUIRE(result.region_colors[br].r == 255);
      REQUIRE(result.region_colors[center].b == 255);
    }
  }

  SECTION("Noise reduction and merging enabled") {
    SECTION("Case 1: Single-pixel speckle absorption") {
      config.min_area = 2;

      QuantizedImage qimg{.w = 3,
                          .h = 3,
                          .pixel_labels = {RED_LABEL, RED_LABEL, RED_LABEL,
                                           RED_LABEL, BLUE_LABEL, RED_LABEL,
                                           RED_LABEL, RED_LABEL, RED_LABEL},
                          .palette = TEST_PALETTE};

      ImageRegions result = Stage::UnionFind::process(qimg, config);

      REQUIRE(result.region_colors.size() == 1);

      int single_reg_id = result.pixel_regions[0][0];
      for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 3; ++x) {
          REQUIRE(result.pixel_regions[y][x] == single_reg_id);
        }
      }

      REQUIRE(result.region_colors[single_reg_id].r == 255);
    }

    SECTION("Case 2: Selection of largest bordering neighbor") {
      config.min_area = 2;

      QuantizedImage qimg{
          .w = 3,
          .h = 3,
          .pixel_labels = {RED_LABEL, RED_LABEL, RED_LABEL, GREEN_LABEL,
                           GREEN_LABEL, BLUE_LABEL, GREEN_LABEL, GREEN_LABEL,
                           GREEN_LABEL},
          .palette = TEST_PALETTE};

      ImageRegions result = Stage::UnionFind::process(qimg, config);

      REQUIRE(result.region_colors.size() == 2);

      int red_reg_id = result.pixel_regions[0][0];
      int green_reg_id = result.pixel_regions[1][0];

      REQUIRE(red_reg_id != green_reg_id);

      REQUIRE(result.pixel_regions[1][2] == green_reg_id);
      REQUIRE(result.region_colors[green_reg_id].g == 255);
    }
  }
}
