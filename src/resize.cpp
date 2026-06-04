#include "resize.hpp"
#include <print>

namespace Stage::Resize {
void process(Image& image, const Config& config) {
  if (config.max_width <= 0 || image.w <= config.max_width) {
    return;
  }

  int new_w = config.max_width;
  int new_h = (image.h * config.max_width) / image.w;
  if (new_h < 1) {
    new_h = 1;
  }

  if (config.verbose) {
    std::println("Resizing image from {}x{} to {}x{}", image.w, image.h, new_w,
                 new_h);
  }

  for (int y = 0; y < new_h; ++y) {
    int src_y = (y * image.h) / new_h;
    for (int x = 0; x < new_w; ++x) {
      int src_x = (x * image.w) / new_w;
      int src_idx = (src_y * image.w + src_x) * 3;
      int dst_idx = (y * new_w + x) * 3;
      image.data[dst_idx] = image.data[src_idx];
      image.data[dst_idx + 1] = image.data[src_idx + 1];
      image.data[dst_idx + 2] = image.data[src_idx + 2];
    }
  }

  image.w = new_w;
  image.h = new_h;
}
}  // namespace Stage::Resize
