#include "utils.hpp"

namespace gro4t {

std::mt19937 CircleProps::generator(std::random_device{}());
std::uniform_int_distribution<int> CircleProps::uniform_int_dist(0, int_dist_range);
std::uniform_real_distribution<double> CircleProps::uniform_real_dist(0.0, 1.0);
std::normal_distribution<double> CircleProps::normal_real_dist(0.0, 1.0);

CircleProps::CircleProps(const GeneratedImageProps& image_props): image_props(image_props) {
    radius = uniform_real_dist(generator) * image_props.max_radius;
    if (radius < image_props.min_radius)
        radius = image_props.min_radius;
    float pos_x = uniform_real_dist(generator) * (image_props.width + 2 * radius) - 2 * radius;
    float pos_y = uniform_real_dist(generator) * (image_props.height + 2 * radius) - 2 * radius;
    position = sf::Vector2f(pos_x, pos_y);
    int r = uniform_int_dist(generator) % 256;
    int g = uniform_int_dist(generator) % 256;
    int b = uniform_int_dist(generator) % 256;
    color = sf::Color(r, g, b);
}

void CircleProps::mutate(double sigma) {
    auto r = normal_real_dist(generator);
    if (std::abs(r) < 0.7) {
        auto prop_number = uniform_int_dist(generator) % 3;
        switch (prop_number) {
            case 0:
                mutateColor(sigma);
                break;
            case 1:
                mutatePosition(sigma);
                break;
            case 2:
                mutateRadius(sigma);
                break;
        }
    }
    else if (std::abs(r) < 1.0) {
        auto pair_number = uniform_int_dist(generator) % 3;
        switch (pair_number) {
            case 0:
                mutateColor(sigma);
                mutatePosition(sigma);
                break;
            case 1:
                mutateColor(sigma);
                mutateRadius(sigma);
                break;
            case 2:
                mutatePosition(sigma);
                mutateRadius(sigma);
                break;
        }
    }
    else {
        mutateColor(sigma);
        mutatePosition(sigma);
        mutateRadius(sigma);
    }
}

void CircleProps::mutateColor(double sigma) {
    const int color_component = uniform_int_dist(generator) % 3;
    const double amount = 10 * sigma * normal_real_dist(generator);
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

void CircleProps::mutatePosition(double sigma) {
    const int axis = uniform_int_dist(generator) % 2;
    double magnitude = 100 * sigma;
    if (magnitude > std::max(image_props.width, image_props.height))
        magnitude = std::max(image_props.width, image_props.height);
    const double amount = magnitude * normal_real_dist(generator);
    if (axis == 0)
        position.x += amount;
    else
        position.y += amount;
    if (position.x < -2 * radius)
        position.x = -2 * radius;
    if (position.y < -2 * radius)
        position.y = -2 * radius;
    if (position.x > image_props.width + 2 * radius)
        position.x = image_props.width + 2 * radius;
    if (position.y > image_props.height + 2 * radius)
        position.y = image_props.height + 2 * radius;
}

void CircleProps::mutateRadius(double sigma) {
    const int direction = uniform_int_dist(generator) % 2 == 0 ? -1 : 1;
    double magnitude = 50 * sigma;
    if (magnitude > image_props.max_radius)
        magnitude = image_props.max_radius;
    const double amount = magnitude * normal_real_dist(generator);
    radius += amount * direction;
    if (radius < image_props.min_radius)
        radius = image_props.min_radius;
    else if (radius > image_props.max_radius)
        radius = image_props.max_radius;
}

GeneratedImageProps::GeneratedImageProps(int circles_num, const sf::Image& original_image, float max_radius, float min_radius)
    : circles_num(circles_num), max_radius(max_radius), min_radius(min_radius) {
    const auto original_image_size = original_image.getSize();
    width = original_image_size.x;
    height = original_image_size.y;
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