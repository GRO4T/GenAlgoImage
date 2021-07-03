#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <optional>
#include <random>

namespace gro4t {
using RenderTexturePtr = std::unique_ptr<sf::RenderTexture>;

inline sf::Color distance(const sf::Color& color_a, const sf::Color& color_b) {
    auto color_distance = [](uint8_t a, uint8_t b) {
        const auto diff = std::abs(a - b);
        return (uint8_t)std::min(diff, 255 - diff);
    };
    const auto diff_r = color_distance(color_a.r, color_b.r);
    const auto diff_g = color_distance(color_a.g, color_b.g);
    const auto diff_b = color_distance(color_a.b, color_b.b);
    return {diff_r, diff_g, diff_b};
}

inline std::ostream& operator<<(std::ostream& os, const sf::Color& color) {
    os << "(" << (int)color.r << ", " << (int)color.g << ", " << (int)color.b << ")";
    return os;
}

struct CircleProp {
    float radius;
    sf::Vector2f position;
    sf::Color color;
};

}  // namespace gro4t