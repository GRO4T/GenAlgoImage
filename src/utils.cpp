#include "utils.hpp"

namespace gro4t {

int GeneratedImage::next_id = 0;

std::vector<CircleProp> getRandomCircleList(int n, int image_width, int image_height, float max_radius) {
    std::vector<CircleProp> circle_prop_list;
    std::mt19937 generator(time(nullptr));
    std::uniform_real_distribution<float> real_dist(0.0f, 1.0f);
    std::uniform_int_distribution<int> int_dist(0, 255);
    for (int i = 0; i < n; ++i) {
        float radius = real_dist(generator) * max_radius;
        float pos_x = real_dist(generator) * (image_width + 2 * radius) - 2 * radius;
        float pos_y = real_dist(generator) * (image_height + 2 * radius) - 2 * radius;
        int r = int_dist(generator);
        int g = int_dist(generator);
        int b = int_dist(generator);
        circle_prop_list.push_back({radius, {pos_x, pos_y}, sf::Color(r, g, b)});
    }
    return circle_prop_list;
}

GeneratedImage generateRandomImage(int circles_num, float max_radius, int image_width, int image_height) {
    auto circle_prop_list = getRandomCircleList(circles_num, image_width, image_height, max_radius);
    auto renderTexture = std::make_unique<sf::RenderTexture>();
    if (!renderTexture->create(image_width, image_height))
        throw std::runtime_error("error creating render texture");
    renderTexture->clear();
    for (const auto& circleProp : circle_prop_list) {
        sf::CircleShape circle(circleProp.radius);
        circle.setPosition(circleProp.position);
        circle.setFillColor(circleProp.color);
        renderTexture->draw(circle);
    }
    renderTexture->display();
    return { renderTexture, renderTexture->getTexture().copyToImage() };
}
}
