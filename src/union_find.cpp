#include <print>
#include <unordered_map>
#include "types.hpp"
namespace Stage::UnionFind {

namespace {
class UnionFind {
  std::vector<int> parent;

 public:
  UnionFind(int size) {
    parent.resize(size);
    for (int i = 0; i < size; i++) {
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
    parent[irep] = jrep;
  }
};
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
        int label = image.pixel_labels[i];
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
