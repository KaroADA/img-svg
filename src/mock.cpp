#include <algorithm>
#include "types.hpp"

ImageRegions mock_segmentation(const Image& img) {
  ImageRegions result;
  result.pixel_regions.resize(img.h, std::vector<int>(img.w, 0));

  // 2 regions: Top = Red, Bottom = Blue
  result.region_colors[0] = Color{255, 0, 0};
  result.region_colors[1] = Color{0, 0, 255};

  for (int y = 0; y < img.h; ++y) {
    for (int x = 0; x < img.w; ++x) {
      if (y < img.h / 2) {
        result.pixel_regions[y][x] = 0;  // Top half
      } else {
        result.pixel_regions[y][x] = 1;  // Bottom half
      }
    }
  }

  return result;
}

ImageRegions mock_segmentation_geom(const Image& img) {
  ImageRegions result;
  result.pixel_regions.resize(img.h, std::vector<int>(img.w, 0));

  // 3 Regions
  result.region_colors[0] = Color{240, 240, 240};
  result.region_colors[1] = Color{255, 50, 50};
  result.region_colors[2] = Color{50, 50, 255};

  // Circle math
  int cx = img.w / 2;
  int cy = img.h / 2;
  int r = std::min(img.w, img.h) / 4;

  // Box math
  int bx1 = img.w / 8;
  int by1 = img.h / 8;
  int bx2 = bx1 + img.w / 4;
  int by2 = by1 + img.h / 4;

  for (int y = 0; y < img.h; ++y) {
    for (int x = 0; x < img.w; ++x) {

      int dx = x - cx;
      int dy = y - cy;

      if (dx * dx + dy * dy <= r * r) {
        // Inside Circle
        result.pixel_regions[y][x] = 1;
      } else if (x >= bx1 && x <= bx2 && y >= by1 && y <= by2) {
        // Inside Box
        result.pixel_regions[y][x] = 2;
      } else {
        // Background
        result.pixel_regions[y][x] = 0;
      }
    }
  }

  return result;
}
