#include "utils.hpp"

#include <functional>

namespace gro4t {

GeneratedImageProps::GeneratedImageProps(int circles_num, const sf::Image& original_image,
                                         float max_radius, float min_radius)
    : max_circles(circles_num), max_radius(max_radius), min_radius(min_radius) {
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

}  // namespace gro4t