#include "circle_props.hpp"

namespace gro4t {

std::mt19937 CircleProps::generator(std::random_device{}());
std::uniform_int_distribution<int> CircleProps::uniform_int_dist(0, 1000);
std::uniform_real_distribution<double> CircleProps::uniform_real_dist(0.0, 1.0);
std::normal_distribution<double> CircleProps::normal_real_dist(0.0, 1.0);

CircleProps::CircleProps(const GeneratedImageProps& image_props) : image_props(image_props) {
    radius = uniform_real_dist(generator) * image_props.max_radius;
    if (radius < image_props.min_radius) radius = image_props.min_radius;
    float pos_x = uniform_real_dist(generator) * (image_props.width + 2 * radius) - 2 * radius;
    float pos_y = uniform_real_dist(generator) * (image_props.height + 2 * radius) - 2 * radius;
    position = sf::Vector2f(pos_x, pos_y);
    int r = uniform_int_dist(generator) % 256;
    int g = uniform_int_dist(generator) % 256;
    int b = uniform_int_dist(generator) % 256;
    color = sf::Color(r, g, b);
}

int CircleProps::getRandomNumber3(double prob_1, double prob_2) {
    const double r = uniform_real_dist(generator);
    if (r < prob_1) return 1;
    if (r < prob_1 + prob_2) return 2;
    return 3;
}

int CircleProps::getRandomNumber2(double prob_1) {
    const double r = uniform_real_dist(generator);
    if (r < prob_1) return 1;
    return 2;
}

void CircleProps::mutate(double sigma) {
    const std::vector<std::function<void()>> mutation_set = {
        [sigma, this]() { mutateColor(sigma); },
        [sigma, this]() { mutatePosition(sigma); },
        [sigma, this]() { mutateRadius(sigma); },
    };
    std::vector<std::function<void()>> mutations_to_call;
    std::sample(mutation_set.begin(), mutation_set.end(), std::back_inserter(mutations_to_call),
                getRandomNumber3(0.6, 0.3), generator);
    for (const auto& mutation_func : mutations_to_call) {
        mutation_func();
    }
}

void CircleProps::mutateColor(double sigma) {
    const std::vector<std::function<void(double)>> color_modifier_set = {
        [this](double amount) { color.r += amount; },
        [this](double amount) { color.g += amount; },
        [this](double amount) { color.b += amount; }};
    std::vector<std::function<void(double)>> modifiers_to_call;
    std::sample(color_modifier_set.begin(), color_modifier_set.end(),
                std::back_inserter(modifiers_to_call), getRandomNumber3(0.6, 0.3), generator);
    for (const auto& color_modifier : modifiers_to_call) {
        const double amount = 10 * sigma * normal_real_dist(generator);
        color_modifier(amount);
    }
}

void CircleProps::mutatePosition(double sigma) {
    double magnitude = 100 * sigma;
    if (magnitude > std::max(image_props.width, image_props.height))
        magnitude = std::max(image_props.width, image_props.height);

    const std::vector<std::function<void()>> position_modifier_set = {
        [this, magnitude] {
          const double amount = magnitude * normal_real_dist(generator);
          position.x += amount;
        },
        [this, magnitude] {
          const double amount = magnitude * normal_real_dist(generator);
          position.y += amount;
        }
    };

    std::vector<std::function<void()>> modifiers_to_call;
    std::sample(position_modifier_set.begin(), position_modifier_set.end(),
                std::back_inserter(modifiers_to_call), getRandomNumber2(0.7), generator);
    for (const auto& position_modifier : modifiers_to_call) position_modifier();

    position.x = clamp(position.x, -2 * radius, image_props.width + 2 * radius);
    position.y = clamp(position.y, -2 * radius, image_props.height + 2 * radius);
}

void CircleProps::mutateRadius(double sigma) {
    const int direction = uniform_int_dist(generator) % 2 == 0 ? -1 : 1;
    double magnitude = 50 * sigma;
    if (magnitude > image_props.max_radius) magnitude = image_props.max_radius;
    const double amount = magnitude * normal_real_dist(generator);
    radius += amount * direction;
    radius = clamp(radius, image_props.max_radius, image_props.max_radius);
}

CircleProps::CircleProps(float radius, const sf::Vector2f& position, const sf::Color& color,
                         const GeneratedImageProps& image_props)
    : radius(radius), position(position), color(color), image_props(image_props) {}

}