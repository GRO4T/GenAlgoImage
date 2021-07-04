#include "utils.hpp"

namespace gro4t {
std::mt19937 CircleProps::generator(std::random_device{}());
std::uniform_int_distribution<int> CircleProps::uniform_int_dist(0, int_dist_range);
std::uniform_real_distribution<double> CircleProps::uniform_real_dist(0.0, 1.0);
std::normal_distribution<> CircleProps::color_dist(0, color_dist_stdev);
std::normal_distribution<> CircleProps::position_dist(0, position_dist_stdev);
std::normal_distribution<> CircleProps::radius_dist(0, radius_dist_stdev);

CircleProps::CircleProps(const GeneratedImageProps& image_props): image_props(image_props) {
    radius = uniform_real_dist(generator) * image_props.max_radius;
    float pos_x = uniform_real_dist(generator) * (image_props.width + 2 * radius) - 2 * radius;
    float pos_y = uniform_real_dist(generator) * (image_props.height + 2 * radius) - 2 * radius;
    position = sf::Vector2f(pos_x, pos_y);
    int r = uniform_int_dist(generator) % 256;
    int g = uniform_int_dist(generator) % 256;
    int b = uniform_int_dist(generator) % 256;
    color = sf::Color(r, g, b);
}
}