#include "path_optimization.hpp"
#include <cmath>
#include <print>

namespace Stage::PathOptimization {

namespace {

double perpendicular_distance(const Point& pt, const Point& line_start,
                              const Point& line_end) {
  double dx = line_end.x - line_start.x;
  double dy = line_end.y - line_start.y;

  if (dx == 0.0 && dy == 0.0) {
    double diff_x = pt.x - line_start.x;
    double diff_y = pt.y - line_start.y;
    return std::sqrt(diff_x * diff_x + diff_y * diff_y);
  }

  double num = std::abs(dy * pt.x - dx * pt.y + line_end.x * line_start.y -
                        line_end.y * line_start.x);
  double den = std::sqrt(dx * dx + dy * dy);
  return num / den;
}

void rdp_recursive(const std::vector<Point>& points, int start_idx, int end_idx,
                   double tolerance, std::vector<bool>& keep_point) {
  double max_dist = 0.0;
  int index = start_idx;

  for (int i = start_idx + 1; i < end_idx; ++i) {
    double dist =
        perpendicular_distance(points[i], points[start_idx], points[end_idx]);
    if (dist > max_dist) {
      max_dist = dist;
      index = i;
    }
  }

  if (max_dist > tolerance) {
    keep_point[index] = true;
    rdp_recursive(points, start_idx, index, tolerance, keep_point);
    rdp_recursive(points, index, end_idx, tolerance, keep_point);
  }
}

Polygon simplify_polygon(const Polygon& polygon, double tolerance) {
  if (polygon.points.size() <= 2)
    return polygon;

  std::vector<bool> keep_point(polygon.points.size(), false);
  keep_point.front() = true;
  keep_point.back() = true;

  rdp_recursive(polygon.points, 0, polygon.points.size() - 1, tolerance,
                keep_point);

  Polygon simplified;
  simplified.color = polygon.color;
  for (size_t i = 0; i < polygon.points.size(); ++i) {
    if (keep_point[i]) {
      simplified.points.push_back(polygon.points[i]);
    }
  }
  return simplified;
}

}  // namespace

std::vector<Polygon> process(const std::vector<Polygon>& polygons,
                             const Config& config) {
  if (config.verbose) {
    std::println("Processing path optimization with tolerance: {}",
                 config.tolerance);
  }

  std::vector<Polygon> optimized_polygons;
  optimized_polygons.reserve(polygons.size());

  int total_original_points = 0;
  int total_optimized_points = 0;

  for (const auto& poly : polygons) {
    total_original_points += poly.points.size();

    Polygon optimized = simplify_polygon(poly, config.tolerance);

    total_optimized_points += optimized.points.size();
    optimized_polygons.push_back(optimized);
  }

  if (config.verbose) {
    std::println("Path optimization completed:");
    std::println("  Original points total:  {}", total_original_points);
    std::println("  Optimized points total: {}", total_optimized_points);
  }

  return optimized_polygons;
}

}  // namespace Stage::PathOptimization
