#include "utils.hpp"

namespace gro4t {
std::mt19937 CircleProps::generator(std::random_device{}());
std::uniform_int_distribution<int> CircleProps::uniform_int_dist(0, int_dist_range);
std::normal_distribution<> CircleProps::color_dist(0, color_dist_stdev);
std::normal_distribution<> CircleProps::position_dist(0, position_dist_stdev);
std::normal_distribution<> CircleProps::radius_dist(0, radius_dist_stdev);
}