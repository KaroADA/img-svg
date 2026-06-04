#include "resize.hpp"
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <cstdlib>
#include <vector>

#define RGB_RED 255, 0, 0
#define RGB_GREEN 0, 255, 0
#define RGB_BLUE 0, 0, 255
#define RGB_WHITE 255, 255, 255
#define RGB_BLACK 0, 0, 0

#define REQUIRE_COLOR(ptr, idx, r, g, b)  \
  do {                                    \
    REQUIRE((ptr)[(idx) * 3] == (r));     \
    REQUIRE((ptr)[(idx) * 3 + 1] == (g)); \
    REQUIRE((ptr)[(idx) * 3 + 2] == (b)); \
  } while (0)

#define REQUIRE_RED(ptr, idx) REQUIRE_COLOR(ptr, idx, 255, 0, 0)
#define REQUIRE_GREEN(ptr, idx) REQUIRE_COLOR(ptr, idx, 0, 255, 0)
#define REQUIRE_BLUE(ptr, idx) REQUIRE_COLOR(ptr, idx, 0, 0, 255)
#define REQUIRE_WHITE(ptr, idx) REQUIRE_COLOR(ptr, idx, 255, 255, 255)

namespace {
Image create_mock_image(int w, int h, const std::vector<uint8_t>& source_data) {
  uint8_t* data = static_cast<uint8_t*>(std::malloc(source_data.size()));
  if (data != nullptr) {
    std::copy(source_data.begin(), source_data.end(), data);
  }
  return Image{.w = w, .h = h, .data = data};
}
}  // namespace

TEST_CASE("Resize stage downscales images based on max_width", "[resize]") {
  Config config{.output_path = "output.svg",
                .max_width = 0,
                .colors = 16,
                .min_area = 10,
                .tolerance = 1.0,
                .corner_threshold = 125.0,
                .optimize = 1.3,
                .verbose = false};

  SECTION(
      "Case 1: No resizing occurs when max_width is disabled or larger than "
      "current width") {
    std::vector<uint8_t> source = {RGB_RED, RGB_BLUE, RGB_GREEN, RGB_WHITE};
    Image img = create_mock_image(2, 2, source);
    uint8_t* original_ptr = img.data;

    config.max_width = 0;
    Stage::Resize::process(img, config);
    REQUIRE(img.w == 2);
    REQUIRE(img.h == 2);
    REQUIRE(img.data == original_ptr);

    config.max_width = 4;
    Stage::Resize::process(img, config);
    REQUIRE(img.w == 2);
    REQUIRE(img.h == 2);
    REQUIRE(img.data == original_ptr);

    std::free(img.data);
  }

  SECTION("Case 2: Standard downscaling of a 4x4 image to 2x2") {
    std::vector<uint8_t> source = {RGB_RED,   RGB_RED,   RGB_BLUE,  RGB_BLUE,
                                   RGB_RED,   RGB_RED,   RGB_BLUE,  RGB_BLUE,
                                   RGB_GREEN, RGB_GREEN, RGB_WHITE, RGB_WHITE,
                                   RGB_GREEN, RGB_GREEN, RGB_WHITE, RGB_WHITE};
    Image img = create_mock_image(4, 4, source);

    config.max_width = 2;
    Stage::Resize::process(img, config);

    REQUIRE(img.w == 2);
    REQUIRE(img.h == 2);
    REQUIRE(img.data != nullptr);

    REQUIRE_RED(img.data, 0);
    REQUIRE_BLUE(img.data, 1);
    REQUIRE_GREEN(img.data, 2);
    REQUIRE_WHITE(img.data, 3);

    std::free(img.data);
  }

  SECTION("Case 3: Extreme aspect ratio scaling down to 1px width") {
    std::vector<uint8_t> source = {RGB_RED,   RGB_GREEN, RGB_BLUE,  RGB_WHITE,
                                   RGB_BLACK, RGB_RED,   RGB_GREEN, RGB_BLUE,
                                   RGB_WHITE, RGB_BLACK};
    Image img = create_mock_image(5, 2, source);

    config.max_width = 1;
    Stage::Resize::process(img, config);

    REQUIRE(img.w == 1);
    REQUIRE(img.h == 1);
    REQUIRE(img.data != nullptr);

    REQUIRE_RED(img.data, 0);

    std::free(img.data);
  }
}
