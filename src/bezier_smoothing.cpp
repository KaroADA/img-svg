#include "bezier_smoothing.hpp"
#include <algorithm>
#include <cmath>
#include <numbers>
#include <print>

namespace Stage::BezierSmoothing {

namespace {
double distance(const Point& a, const Point& b) {
  return std::hypot(a.x - b.x, a.y - b.y);
}
}  // namespace

std::vector<Shape> process(const std::vector<Polygon>& polygons,
                           const Config& config) {
  if (config.verbose) {
    std::println("Processing Bezier smoothing with parameters:");
    std::println("  corner_threshold: {}", config.corner_threshold);
    std::println("  optimize: {}", config.optimize);
  }

  std::vector<Shape> shapes;
  shapes.reserve(polygons.size());

  for (const auto& poly : polygons) {
    Shape shape;
    shape.color = poly.color;
    const int n = poly.points.size();

    if (n < 3) {
      for (const auto& pt : poly.points) {
        shape.nodes.push_back(PathNode{
            .position = {static_cast<double>(pt.x), static_cast<double>(pt.y)},
            .is_curve = false});
      }
      shapes.push_back(shape);
      continue;
    }

    std::vector<bool> smooth(n, false);
    std::vector<Vec2> forward(n);
    std::vector<Vec2> backward(n);

    for (int i = 0; i < n; ++i) {
      int prev = (i - 1 + n) % n;
      int next = (i + 1) % n;

      Point A = poly.points[prev];
      Point B = poly.points[i];
      Point C = poly.points[next];

      double lenBA = distance(B, A);
      double lenBC = distance(B, C);

      if (lenBA == 0.0 || lenBC == 0.0) {
        smooth[i] = false;
      } else {
        double dot = (A.x - B.x) * (C.x - B.x) + (A.y - B.y) * (C.y - B.y);
        double cos_val = std::clamp(dot / (lenBA * lenBC), -1.0, 1.0);
        double angle = std::acos(cos_val) * 180.0 / std::numbers::pi;

        smooth[i] = (angle > config.corner_threshold);
      }

      if (smooth[i]) {
        double tx = C.x - A.x;
        double ty = C.y - A.y;
        double lenT = std::hypot(tx, ty);

        if (lenT > 0.0) {
          tx /= lenT;
          ty /= lenT;
        }

        double factor = config.optimize * 0.25;
        forward[i] = {B.x + tx * lenBC * factor, B.y + ty * lenBC * factor};
        backward[i] = {B.x - tx * lenBA * factor, B.y - ty * lenBA * factor};
      } else {
        forward[i] = {static_cast<double>(B.x), static_cast<double>(B.y)};
        backward[i] = {static_cast<double>(B.x), static_cast<double>(B.y)};
      }
    }

    for (int i = 0; i < n; ++i) {
      int prev = (i - 1 + n) % n;
      Point B = poly.points[i];

      PathNode node;
      node.position = {static_cast<double>(B.x), static_cast<double>(B.y)};

      if (smooth[prev] || smooth[i]) {
        node.is_curve = true;
        node.control1 = forward[prev];
        node.control2 = backward[i];
      } else {
        node.is_curve = false;
      }

      shape.nodes.push_back(node);
    }

    shapes.push_back(shape);
  }

  if (config.verbose) {
    std::println("Bezier smoothing completed. Generated {} shapes.",
                 shapes.size());
  }

  return shapes;
}

}  // namespace Stage::BezierSmoothing
