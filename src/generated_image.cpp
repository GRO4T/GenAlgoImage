#include "generated_image.hpp"

namespace gro4t {
int GeneratedImage::next_id = 0;
std::mt19937 GeneratedImage::generator(std::random_device{}());
std::uniform_real_distribution<double> GeneratedImage::real_dist(0.0, 1.0);
std::uniform_int_distribution<int> GeneratedImage::int_dist(0, int_dist_range);
}
