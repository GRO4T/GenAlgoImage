#include "image_generator.hpp"

namespace gro4t {

std::vector<CircleProp> ImageGenerator::getRandomCircleList(int n, int image_width, int image_height,
                                            float max_radius) {
    std::vector<CircleProp> circle_prop_list;
    for (int i = 0; i < n; ++i) {
        float radius = real_dist(generator) * max_radius;
        float pos_x = real_dist(generator) * (image_width + 2 * radius) - 2 * radius;
        float pos_y = real_dist(generator) * (image_height + 2 * radius) - 2 * radius;
        int r = int_dist(generator);
        int g = int_dist(generator);
        int b = int_dist(generator);
        circle_prop_list.push_back({radius, {pos_x, pos_y}, sf::Color(r, g, b)});
    }
    return circle_prop_list;
}

}  // namespace gro4t