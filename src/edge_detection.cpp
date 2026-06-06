#include "edge_detection.hpp"
#include <print>
#include <vector>

namespace Stage::EdgeDetection {

namespace {
constexpr int DIR_X[8] = {1, 1, 0, -1, -1, -1, 0, 1};
constexpr int DIR_Y[8] = {0, 1, 1, 1, 0, -1, -1, -1};

bool is_edge_pixel(const ImageRegions& regions, int x, int y) {
  const int h = regions.pixel_regions.size();
  const int w = regions.pixel_regions[0].size();
  const int region_id = regions.pixel_regions[y][x];

  if (x == 0 || y == 0 || x == w - 1 || y == h - 1) {
    return true;
  }

  if (regions.pixel_regions[y][x + 1] != region_id)
    return true;
  if (regions.pixel_regions[y + 1][x] != region_id)
    return true;
  if (regions.pixel_regions[y][x - 1] != region_id)
    return true;
  if (regions.pixel_regions[y - 1][x] != region_id)
    return true;

  return false;
}

Polygon trace_polygon(const ImageRegions& regions, int start_x, int start_y,
                      std::vector<bool>& visited) {
  Polygon polygon;
  const int h = regions.pixel_regions.size();
  const int w = regions.pixel_regions[0].size();
  const int region_id = regions.pixel_regions[start_y][start_x];

  int x = start_x;
  int y = start_y;
  int start_dir = -1;
  int dir = 5;

  do {
    polygon.points.emplace_back(x, y);

    bool found_next = false;
    for (int i = 0; i < 8; ++i) {
      int new_dir = (dir + i) % 8;
      int nx = x + DIR_X[new_dir];
      int ny = y + DIR_Y[new_dir];

      if (nx >= 0 && nx < w && ny >= 0 && ny < h &&
          regions.pixel_regions[ny][nx] == region_id &&
          is_edge_pixel(regions, nx, ny)) {
        if (!visited[ny * w + nx] || (nx == start_x && ny == start_y)) {

          if (nx == start_x && ny == start_y) {
            break;
          }

          x = nx;
          y = ny;

          if (start_dir == -1) {
            start_dir = new_dir;
          }

          dir = (new_dir + 5) % 8;
          found_next = true;
          break;
        }
      }
    }

    if (!found_next) {
      break;
    }
  } while (true);

  for (const auto& pt : polygon.points) {
    visited[pt.y * w + pt.x] = true;
  }

  if (regions.region_colors.contains(region_id)) {
    polygon.color = regions.region_colors.at(region_id);
  }

  return polygon;
}
}  // namespace

std::vector<Polygon> process(const ImageRegions& regions,
                             const Config& config) {
  if (config.verbose) {
    std::println("Processing edge detection with parameters:");
    std::println("  min_area: {}", config.min_area);
  }

  std::vector<Polygon> all_polygons;

  const int h = regions.pixel_regions.size();
  const int w = regions.pixel_regions[0].size();

  std::vector<bool> visited(h * w, false);

  int total_traced = 0;

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      if (!visited[y * w + x] && is_edge_pixel(regions, x, y)) {
        Polygon polygon = trace_polygon(regions, x, y, visited);
        total_traced++;
        all_polygons.push_back(polygon);
        if (config.verbose) {
          std::println(
              "  [Polygon {}] Found at ({}, {}), points: {}, color: "
              "#{:02x}{:02x}{:02x}",
              all_polygons.size(), x, y, polygon.points.size(), polygon.color.r,
              polygon.color.g, polygon.color.b);
        }
      }
    }
  }

  if (config.verbose) {
    std::println("Edge detection completed:");
    std::println("  Total contours traced: {}", total_traced);
    std::println("  Polygons kept:         {}", all_polygons.size());
  }

  return all_polygons;
}
}  // namespace Stage::EdgeDetection
