/**
 * @file types.hpp
 * @brief Common data structures for image processing and vectorization.
 */

#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief Global configuration settings.
 */
struct Config {
  std::string output_path;  ///< Path where the output file will be saved.
  int max_width;            ///< Maximum allowed image width in pixels.
  int colors;               ///< Target number of colors for quantization.
  int min_area;             ///< Minimum pixel area for a region to be kept.
  double tolerance;         ///< Maximum distance for path simplification.
  double corner_threshold;  ///< Angle threshold for smoothing.
  double smoothing;         ///< Multiplier for curve smoothing intensity.
  bool verbose;             ///< Enables detailed logging to stdout.
};

/**
 * @brief RGB color representation.
 */
struct Color {
  uint8_t r;  ///< Red channel (0-255).
  uint8_t g;  ///< Green channel (0-255).
  uint8_t b;  ///< Blue channel (0-255).
};

/**
 * @brief Raw image data.
 */
struct Image {
  int w;          ///< Image width in pixels.
  int h;          ///< Image height in pixels.
  uint8_t* data;  ///< Pointer to interleaved RGB pixel data.
};

/**
 * @brief Image after color quantization.
 */
struct QuantizedImage {
  int w;                          ///< Image width in pixels.
  int h;                          ///< Image height in pixels.
  std::vector<int> pixel_labels;  ///< Cluster index for each pixel.
  std::vector<Color> palette;     ///< List of colors used in the image.
};

/**
 * @brief Pixels grouped into connected regions.
 */
struct ImageRegions {
  std::vector<std::vector<int>> pixel_regions;   ///< 2D grid of region IDs.
  std::unordered_map<int, Color> region_colors;  ///< Maps region IDs to colors.
};

/**
 * @brief Integer coordinates of a point.
 */
struct Point {
  int x;  ///< Horizontal coordinate.
  int y;  ///< Vertical coordinate.
};

/**
 * @brief Closed path made of points and a color.
 */
struct Polygon {
  std::vector<Point> points;  ///< Ordered list of vertices.
  Color color;                ///< Fill color of the polygon.
};

/**
 * @brief 2D vector with double precision.
 */
struct Vec2 {
  double x;  ///< Horizontal component.
  double y;  ///< Vertical component.
};

/**
 * @brief Single node in a vector path.
 */
struct PathNode {
  Vec2 position;    ///< Main point position.
  Vec2 control1{};  ///< First Bezier control point.
  Vec2 control2{};  ///< Second Bezier control point.
  bool is_curve{
      false};  ///< True if the segment leading to this node is a curve.
};

/**
 * @brief A vector shape composed of path nodes.
 */
struct Shape {
  std::vector<PathNode>
      nodes;    ///< Sequence of points and curves defining the outline.
  Color color;  ///< Fill color of the shape.
};
