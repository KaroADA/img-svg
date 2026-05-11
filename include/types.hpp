#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

struct Image {
  int w;
  int h;
  uint8_t* data;
};

struct ImageRegions {
  std::vector<std::vector<int>> pixel_regions;
  std::unordered_map<int, Color> region_colors;
};
