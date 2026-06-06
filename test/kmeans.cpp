#include "kmeans.hpp"
#include <catch2/catch_test_macros.hpp>
#include <cstdlib>
#include <vector>

#define RGB_RED 255, 0, 0
#define RGB_GREEN 0, 255, 0
#define RGB_BLUE 0, 0, 255
#define RGB_WHITE 255, 255, 255
#define RGB_BLACK 0, 0, 0

namespace {
// Finds the index of a target RGB color in the palette dynamically.
// This ensures assertions remain deterministic
int find_palette_index(const std::vector<Color>& palette, uint8_t r, uint8_t g,
                       uint8_t b) {
  for (size_t i = 0; i < palette.size(); ++i) {
    if (palette[i].r == r && palette[i].g == g && palette[i].b == b) {
      return static_cast<int>(i);
    }
  }
  return -1;
}
}  // namespace

#define REQUIRE_COLOR(qimg, pixel_idx, r, g, b)                \
  do {                                                         \
    int col_idx = find_palette_index((qimg).palette, r, g, b); \
    REQUIRE(col_idx != -1);                                    \
    REQUIRE((qimg).pixel_labels[(pixel_idx)] == col_idx);      \
  } while (0)

#define REQUIRE_RED(qimg, idx) REQUIRE_COLOR(qimg, idx, 255, 0, 0)
#define REQUIRE_GREEN(qimg, idx) REQUIRE_COLOR(qimg, idx, 0, 255, 0)
#define REQUIRE_BLUE(qimg, idx) REQUIRE_COLOR(qimg, idx, 0, 0, 255)

TEST_CASE("KMeans stage partitions image pixels into K clusters", "[kmeans]") {
  std::srand(1);

  Config config{.output_path = "output.svg",
                .max_width = 0,
                .colors = 2,
                .min_area = 10,
                .tolerance = 1.0,
                .corner_threshold = 125.0,
                .smoothing = 1.3,
                .verbose = false};

  SECTION("Case 1: Standard 2-color partitioning on a 4x4 image") {
    std::vector<uint8_t> pixel_data = {RGB_RED,  RGB_RED,  RGB_RED,  RGB_RED,
                                       RGB_RED,  RGB_RED,  RGB_RED,  RGB_RED,
                                       RGB_BLUE, RGB_BLUE, RGB_BLUE, RGB_BLUE,
                                       RGB_BLUE, RGB_BLUE, RGB_BLUE, RGB_BLUE};

    Image img{.w = 4, .h = 4, .data = pixel_data.data()};

    config.colors = 2;
    QuantizedImage qimg = Stage::KMeans::process(img, config);

    REQUIRE(qimg.w == 4);
    REQUIRE(qimg.h == 4);
    REQUIRE(qimg.palette.size() == 2);

    for (int i = 0; i < 8; ++i) {
      REQUIRE_RED(qimg, i);
    }
    for (int i = 8; i < 16; ++i) {
      REQUIRE_BLUE(qimg, i);
    }
  }

  SECTION("Case 2: Uniform 1-color clustering on a 2x2 image") {
    std::vector<uint8_t> pixel_data = {RGB_GREEN, RGB_GREEN, RGB_GREEN,
                                       RGB_GREEN};

    Image img{.w = 2, .h = 2, .data = pixel_data.data()};

    config.colors = 1;
    QuantizedImage qimg = Stage::KMeans::process(img, config);

    REQUIRE(qimg.w == 2);
    REQUIRE(qimg.h == 2);
    REQUIRE(qimg.palette.size() == 1);

    for (int i = 0; i < 4; ++i) {
      REQUIRE_GREEN(qimg, i);
    }
  }

  SECTION("Case 3: Clean partitioning of 3 distinct horizontal bands") {
    std::vector<uint8_t> pixel_data = {RGB_RED,   RGB_RED,   RGB_RED,
                                       RGB_GREEN, RGB_GREEN, RGB_GREEN,
                                       RGB_BLUE,  RGB_BLUE,  RGB_BLUE};

    Image img{.w = 3, .h = 3, .data = pixel_data.data()};

    config.colors = 3;
    QuantizedImage qimg = Stage::KMeans::process(img, config);

    REQUIRE(qimg.w == 3);
    REQUIRE(qimg.h == 3);
    REQUIRE(qimg.palette.size() == 3);

    for (int i = 0; i < 3; ++i) {
      REQUIRE_RED(qimg, i);
    }
    for (int i = 3; i < 6; ++i) {
      REQUIRE_GREEN(qimg, i);
    }
    for (int i = 6; i < 9; ++i) {
      REQUIRE_BLUE(qimg, i);
    }
  }
}
