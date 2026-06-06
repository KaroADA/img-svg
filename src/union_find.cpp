#include <print>
#include <unordered_map>
#include <vector>
#include "types.hpp"

namespace Stage::UnionFind {

namespace {
class UnionFind {
  std::vector<int> parent;
  std::vector<int> size;

 public:
  UnionFind(int total_size) {
    parent.resize(total_size);
    size.assign(total_size, 1);
    for (int i = 0; i < total_size; i++) {
      parent[i] = i;
    }
  }
  int find(int i) {
    if (parent[i] == i) {
      return i;
    }
    return parent[i] = find(parent[i]);
  }
  void unite(int i, int j) {
    int irep = find(i);
    int jrep = find(j);
    if (irep != jrep) {
      parent[irep] = jrep;
      size[jrep] += size[irep];
    }
  }
  int get_size(int i) { return size[find(i)]; }
};

double get_color_distance(const Color& c1, const Color& c2) {
  double dr = static_cast<double>(c1.r) - c2.r;
  double dg = static_cast<double>(c1.g) - c2.g;
  double db = static_cast<double>(c1.b) - c2.b;
  return dr * dr + dg * dg + db * db;
}
}  // namespace

ImageRegions process(const QuantizedImage& image, const Config& config) {
  if (config.verbose) {
    std::println("Running Union Find.");
  }
  UnionFind uf(image.w * image.h);
  // Unite pixels into regions with union find
  for (int y = 0; y < image.h; y++) {
    for (int x = 0; x < image.w; x++) {
      int i = y * image.w + x;
      int label = image.pixel_labels[i];
      if (x + 1 < image.w) {
        int right_label = image.pixel_labels[i + 1];
        if (right_label == label) {
          uf.unite(i, i + 1);
        }
      }
      if (y + 1 < image.h) {
        int down_label = image.pixel_labels[i + image.w];
        if (down_label == label) {
          uf.unite(i, i + image.w);
        }
      }
    }
  }

  if (config.verbose) {
    std::println("Running Noise Reduction, min-area = {}.", config.min_area);
  }

  // Combine regions smaller than min-area with their biggest neighbors
  int total_pixels = image.w * image.h;
  bool merged_any;
  int pass = 1;

  do {
    merged_any = false;
    std::vector<int> best_neighbor_root(total_pixels, -1);
    std::vector<double> best_neighbor_dist(total_pixels, 1e9);
    std::vector<int> best_neighbor_size(total_pixels, -1);

    for (int y = 0; y < image.h; y++) {
      for (int x = 0; x < image.w; x++) {
        int i = y * image.w + x;
        int root = uf.find(i);
        if (uf.get_size(root) >= config.min_area) {
          continue;
        }
        int neighbors[4] = {
            (x > 0) ? i - 1 : -1, (x + 1 < image.w) ? i + 1 : -1,
            (y > 0) ? i - image.w : -1, (y + 1 < image.h) ? i + image.w : -1};

        Color current_color = image.palette[image.pixel_labels[root]];

        for (int n_idx : neighbors) {
          if (n_idx == -1) {
            continue;
          }
          int n_root = uf.find(n_idx);
          if (n_root == root) {
            continue;
          }

          Color neighbor_color = image.palette[image.pixel_labels[n_root]];
          double dist = get_color_distance(current_color, neighbor_color);
          int n_size = uf.get_size(n_root);

          if (dist < best_neighbor_dist[root] ||
              (dist == best_neighbor_dist[root] &&
               n_size > best_neighbor_size[root])) {
            best_neighbor_dist[root] = dist;
            best_neighbor_size[root] = n_size;
            best_neighbor_root[root] = n_root;
          }
        }
      }
    }

    int unifies_this_pass = 0;
    for (int i = 0; i < total_pixels; i++) {
      if (uf.find(i) == i && uf.get_size(i) < config.min_area) {
        int target = best_neighbor_root[i];
        if (target != -1) {
          int target_root = uf.find(target);
          if (i != target_root) {
            uf.unite(i, target_root);
            merged_any = true;
            unifies_this_pass++;
          }
        }
      }
    }

    if (config.verbose && merged_any) {
      std::println("  Pass {}: unified {} regions.", pass++, unifies_this_pass);
    }

  } while (merged_any);

  // Index the regions sequentially and return
  ImageRegions result;
  result.pixel_regions.resize(image.h);
  std::unordered_map<int, int> root_to_region_id;
  int num_regions = 0;
  for (int y = 0; y < image.h; y++) {
    result.pixel_regions[y].resize(image.w);
    for (int x = 0; x < image.w; x++) {
      int i = y * image.w + x;
      int root = uf.find(i);
      if (!root_to_region_id.contains(root)) {
        root_to_region_id[root] = num_regions;
        int label = image.pixel_labels[root];
        result.region_colors[num_regions] = image.palette[label];
        num_regions++;
      }
      result.pixel_regions[y][x] = root_to_region_id[root];
    }
  }
  if (config.verbose) {
    std::println("Found {} regions.", num_regions);
  }

  return result;
}
}  // namespace Stage::UnionFind
