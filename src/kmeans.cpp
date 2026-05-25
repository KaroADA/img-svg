#include <cmath>
#include <cstdlib>
#include <print>
#include <ranges>
#include <vector>
#include "types.hpp"

namespace Stage::KMeans {

namespace {
struct Cluster {
  Color center;
};

Color get_color(const Image& image, int i) {
  return Color{.r = image.data[3 * i],
               .g = image.data[3 * i + 1],
               .b = image.data[3 * i + 2]};
}

int get_closest_cluster_id(const std::vector<Cluster>& clusters,
                           const Color& col) {
  int best_id = 0;
  int min_dist_sq = 255 * 255 * 3 + 1;

  for (size_t i = 0; i < clusters.size(); ++i) {
    int dr = clusters[i].center.r - col.r;
    int dg = clusters[i].center.g - col.g;
    int db = clusters[i].center.b - col.b;
    int dist_sq = dr * dr + dg * dg + db * db;

    if (dist_sq < min_dist_sq) {
      min_dist_sq = dist_sq;
      best_id = static_cast<int>(i);
    }
  }
  return best_id;
}

void print_clusters(const std::vector<Cluster>& clusters) {
  int column = 0;
  int columns = static_cast<int>(std::sqrt(clusters.size()));
  for (Cluster cluster : clusters) {
    if (column == -1) {
      std::println(",");
      column = 0;
    } else if (column != 0) {
      std::print(", ");
    }
    std::print("RGB({}, {}, {})", cluster.center.r, cluster.center.g,
               cluster.center.b);
    column += 1;
    if (column == columns) {
      column = -1;
    }
  }
  std::println();
}
}  // namespace

QuantizedImage process(const Image& image, const Config& config) {
  if (config.verbose) {
    std::println("Running KMeans, K = {}.", config.colors);
  }
  std::vector<Cluster> clusters;
  clusters.reserve(config.colors);
  int total_points = image.h * image.w;

  // labels[pixel_id] = cluster_id
  std::vector<int> labels(total_points, -1);

  // Initialize random centers
  std::vector<Color> used_colors;
  for (int i = 0; i < config.colors; i++) {
    while (true) {
      int point_id = rand() % total_points;
      Color col = get_color(image, point_id);

      bool used = false;
      for (Color c : used_colors) {
        if (c.r == col.r && c.g == col.g && c.b == col.b) {
          used = true;
          break;
        }
      }

      if (!used) {
        used_colors.push_back(col);
        clusters.push_back(Cluster{.center = col});
        break;
      }
    }
  }
  if (config.verbose) {
    std::println("Clusters initialized:");
    print_clusters(clusters);
  }

  int iter = 1;
  const int max_iters = 100;
  while (iter <= max_iters) {
    bool done = true;

    // Add all points to their nearest cluster
    for (int i = 0; i < total_points; i++) {
      Color col = get_color(image, i);
      int cluster_id = get_closest_cluster_id(clusters, col);

      if (labels[i] != cluster_id) {
        labels[i] = cluster_id;
        done = false;
      }
    }

    if (done)
      break;

    // Recalculate the center of each cluster
    std::vector<long long> sum_r(config.colors, 0);
    std::vector<long long> sum_g(config.colors, 0);
    std::vector<long long> sum_b(config.colors, 0);
    std::vector<int> counts(config.colors, 0);

    for (int i = 0; i < total_points; i++) {
      int c_id = labels[i];
      Color col = get_color(image, i);
      sum_r[c_id] += col.r;
      sum_g[c_id] += col.g;
      sum_b[c_id] += col.b;
      counts[c_id]++;
    }

    for (int c = 0; c < config.colors; c++) {
      if (counts[c] > 0) {
        clusters[c].center.r = static_cast<uint8_t>(sum_r[c] / counts[c]);
        clusters[c].center.g = static_cast<uint8_t>(sum_g[c] / counts[c]);
        clusters[c].center.b = static_cast<uint8_t>(sum_b[c] / counts[c]);
      }
    }

    iter++;
  }

  if (config.verbose) {
    std::println("Clustering done in {} iterations:", iter);
    print_clusters(clusters);
  }

  std::vector<Color> colors =
      clusters |
      std::views::transform([](Cluster& c) { return std::move(c.center); }) |
      std::ranges::to<std::vector>();

  return QuantizedImage{
      .w = image.w, .h = image.h, .pixel_labels = labels, .palette = colors};
}
}  // namespace Stage::KMeans
