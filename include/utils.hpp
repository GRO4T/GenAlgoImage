#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <optional>
#include <random>

namespace gro4t {

using RenderTexturePtr = std::unique_ptr<sf::RenderTexture>;

struct GeneratedImageProps {
    int circles_num;
    float max_radius;
    uint32_t width;
    uint32_t height;

    GeneratedImageProps(int circles_num, const sf::Image& original_image): circles_num(circles_num) {
        const auto original_image_size = original_image.getSize();
        width = original_image_size.x;
        height = original_image_size.y;
        max_radius = std::max(width, height);
    }

    GeneratedImageProps() {}
    ~GeneratedImageProps() {}
};

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

class CircleProps {
public:
    float radius;
    sf::Vector2f position;
    sf::Color color;

    CircleProps(const GeneratedImageProps& image_props);
    ~CircleProps() {}

    void mutate() {
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

private:
    static const int int_dist_range = 1000;
    static std::mt19937 generator;
    static std::uniform_int_distribution<int> uniform_int_dist;
    static std::uniform_real_distribution<double> uniform_real_dist;
    static const int color_dist_stdev = 10;
    static std::normal_distribution<> color_dist;
    static const int position_dist_stdev = 10;
    static std::normal_distribution<> position_dist;
    static const int radius_dist_stdev = 10;
    static std::normal_distribution<> radius_dist;

    GeneratedImageProps image_props;


    void mutateColor() {
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

    void mutatePosition() {
        const int axis = uniform_int_dist(generator) % 2;
        const double amount = position_dist(generator);
        if (axis == 0)
            position.x += amount;
        else
            position.y += amount;
    }

    void mutateRadius() {
        const int direction = uniform_int_dist(generator) % 2 == 0 ? -1 : 1;
        const double amount = radius_dist(generator);
        radius += amount * direction;
        if (radius < 0)
            radius = 0;
        else if (radius > image_props.max_radius)
            radius = image_props.max_radius;
    }
};

}  // namespace gro4t