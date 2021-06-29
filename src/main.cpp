#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <vector>
#include <memory>
#include <random>

using RenderTexturePtr = std::unique_ptr<sf::RenderTexture>;

struct CircleProp {
    float radius;
    sf::Vector2f position;
    sf::Color color;
};

std::vector<CircleProp> getRandomCircleList(int n, int image_width, int image_height, float max_radius) {
    std::vector<CircleProp> cirlePropList;
    std::default_random_engine generator;
    std::uniform_real_distribution<float> real_dist(0.0f, 1.0f);
    for (int i = 0; i < n; ++i) {
        float radius = real_dist(generator) * max_radius;
        float pos_x = real_dist(generator) * (image_width + 2 * radius) - 2 * radius;
        float pos_y = real_dist(generator) * (image_height + 2 * radius) - 2 * radius;
        cirlePropList.push_back({radius, {pos_x, pos_y}, sf::Color::Green});
    }
    return cirlePropList;
}

RenderTexturePtr getRenderTexture(std::vector<CircleProp> circlePropList, int image_width, int image_height) {
    auto renderTexture = std::make_unique<sf::RenderTexture>();
    if (!renderTexture->create(image_width, image_height))
        throw std::runtime_error("error creating render texture");
    renderTexture->clear();
    for (const auto& circleProp : circlePropList) {
        sf::CircleShape circle(circleProp.radius);
        circle.setPosition(circleProp.position);
        circle.setFillColor(circleProp.color);
        renderTexture->draw(circle);
    }
    renderTexture->display();
    return std::move(renderTexture);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(500, 500), "Genetic Image");

//    sf::Texture originalImage;
//    if (!originalImage.loadFromFile("res/lena.png"))
//        throw std::runtime_error("error loading original image");

    auto randomTexture = getRenderTexture(
            getRandomCircleList(10, 500, 500, 100),
            500,
            500)->getTexture();
    sf::Sprite displayedSprite(randomTexture);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(displayedSprite);
        window.display();
    }
    return 0;
}