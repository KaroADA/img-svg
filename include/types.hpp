#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

struct Config {
  std::string output_path;
  int colors;
  int min_area;
  double tolerance;
  double corner_threshold;
  double optimize;
  bool verbose;
};

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

struct QuantizedImage {
  int w;
  int h;
  std::vector<int> pixel_labels;
  std::vector<Color> palette;
};

struct ImageRegions {
  std::vector<std::vector<int>> pixel_regions;
  std::unordered_map<int, Color> region_colors;
};

struct Point {
  int x;
  int y;
};

struct Polygon {
  std::vector<Point> points;
  Color color;
};

struct Vec2 {
  double x;
  double y;
};

struct PathNode {
  Vec2 position;
  Vec2 control1{};
  Vec2 control2{};
  bool is_curve{false};
};

struct Shape {
  std::vector<PathNode> nodes;
  Color color;
};
