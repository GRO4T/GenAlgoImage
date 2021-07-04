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

void CircleProps::mutate() {
    int prop_number = uniform_int_dist(generator) % 3;
    switch (prop_number) {
        case 0:
            mutateColor();
            break;
        case 1:
            mutatePosition();
            break;
        case 2:
            mutateRadius();
            break;
    }
}

void CircleProps::mutateColor() {
    const int color_component = uniform_int_dist(generator) % 3;
    const double amount = color_dist(generator);
    switch (color_component) {
        case 0:
            color.r += amount;
            break;
        case 1:
            color.g += amount;
            break;
        case 2:
            color.b += amount;
            break;
    }
}

void CircleProps::mutatePosition() {
    const int axis = uniform_int_dist(generator) % 2;
    const double amount = position_dist(generator);
    if (axis == 0)
        position.x += amount;
    else
        position.y += amount;
}

void CircleProps::mutateRadius() {
    const int direction = uniform_int_dist(generator) % 2 == 0 ? -1 : 1;
    const double amount = radius_dist(generator);
    radius += amount * direction;
    if (radius < 0)
        radius = 0;
    else if (radius > image_props.max_radius)
        radius = image_props.max_radius;
}

GeneratedImageProps::GeneratedImageProps(int circles_num, const sf::Image& original_image)
    : circles_num(circles_num) {
    const auto original_image_size = original_image.getSize();
    width = original_image_size.x;
    height = original_image_size.y;
    max_radius = std::max(width, height);
}

sf::Color distance(const sf::Color& color_a, const sf::Color& color_b) {
    auto color_distance = [](uint8_t a, uint8_t b) {
      const auto diff = std::abs(a - b);
      return (uint8_t)std::min(diff, 255 - diff);
    };
    const auto diff_r = color_distance(color_a.r, color_b.r);
    const auto diff_g = color_distance(color_a.g, color_b.g);
    const auto diff_b = color_distance(color_a.b, color_b.b);
    return {diff_r, diff_g, diff_b};
}

std::ostream& operator<<(std::ostream& os, const sf::Color& color) {
    os << "(" << (int)color.r << ", " << (int)color.g << ", " << (int)color.b << ")";
    return os;
}

}