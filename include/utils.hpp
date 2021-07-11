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

class CircleProps {
public:
    float radius;
    sf::Vector2f position;
    sf::Color color;

    CircleProps(const GeneratedImageProps& image_props);
    CircleProps(float radius, const sf::Vector2f& position, const sf::Color& color,
                const GeneratedImageProps& image_props);
    ~CircleProps() {}

    void mutate(double sigma);

private:
    static const int int_dist_range = 1000;
    static std::mt19937 generator;
    static std::uniform_int_distribution<int> uniform_int_dist;
    static std::uniform_real_distribution<double> uniform_real_dist;
    static std::normal_distribution<double> normal_real_dist;

    static constexpr double mutation_rate = 0.6;

    GeneratedImageProps image_props;

    void mutateColor(double sigma);
    void mutatePosition(double sigma);
    void mutateRadius(double sigma);

    int getPropNumberNormalDist3();
};

}  // namespace gro4t