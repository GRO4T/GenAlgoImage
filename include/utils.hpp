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

    GeneratedImageProps(int circles_num, const sf::Image& original_image);
    GeneratedImageProps() {}
    ~GeneratedImageProps() {}
};

sf::Color distance(const sf::Color& color_a, const sf::Color& color_b);

std::ostream& operator<<(std::ostream& os, const sf::Color& color);

class CircleProps {
public:
    float radius;
    sf::Vector2f position;
    sf::Color color;

    CircleProps(const GeneratedImageProps& image_props);
    ~CircleProps() {}

    void mutate();

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

    void mutateColor();
    void mutatePosition();
    void mutateRadius();
};

}  // namespace gro4t