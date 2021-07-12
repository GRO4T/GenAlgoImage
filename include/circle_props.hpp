#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <optional>
#include <random>

#include "utils.hpp"

namespace gro4t {

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
    static std::mt19937 generator;
    static std::uniform_int_distribution<int> uniform_int_dist;
    static std::uniform_real_distribution<double> uniform_real_dist;
    static std::normal_distribution<double> normal_real_dist;

    GeneratedImageProps image_props;

    void mutateColor(double sigma);
    void mutatePosition(double sigma);
    void mutateRadius(double sigma);

    int getRandomNumber3(double prob_1, double prob_2);
    int getRandomNumber2(double prob_1);
};

}
