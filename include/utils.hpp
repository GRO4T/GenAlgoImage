#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <optional>
#include <random>

namespace gro4t {

using RenderTexturePtr = std::unique_ptr<sf::RenderTexture>;

struct GeneratedImageProps {
    int max_circles;
    float max_radius;
    float min_radius;
    uint32_t width;
    uint32_t height;

    GeneratedImageProps(int circles_num, const sf::Image& original_image, float max_radius, float min_radius);
    GeneratedImageProps() {}
    ~GeneratedImageProps() {}
};

sf::Color distance(const sf::Color& color_a, const sf::Color& color_b);

std::ostream& operator<<(std::ostream& os, const sf::Color& color);

template<typename T>
inline T clamp(const T& value, const T& min, const T& max) {
    return std::max(std::min(value, max), min);
}

}  // namespace gro4t