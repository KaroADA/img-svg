#pragma once
#include "types.hpp"

namespace Stage::KMeans {
QuantizedImage process(const Image& image, const Config& config);
}
