#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <vector>
#include <memory>
#include <random>
#include <ctime>
#include <iostream>

// evaluation function parameters
const double points_per_exact_match = 1.0;

using RenderTexturePtr = std::unique_ptr<sf::RenderTexture>;

struct CircleProp {
    float radius;
    sf::Vector2f position;
    sf::Color color;
};

class ImageComposite {
public:
    ImageComposite(RenderTexturePtr &renderTexture, const sf::Image &image) : renderTexture(std::move(renderTexture)),
                                                                                    image(image) {}

    const sf::Texture& getTexture() const { return renderTexture->getTexture(); }
    const sf::Image& getImage() const { return image; }
private:
    RenderTexturePtr renderTexture;
    sf::Image image;
};

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

ImageComposite generateRandomImage(int circles_num, float max_radius, int image_width, int image_height) {
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

double evaluate(const sf::Image& original_image, const sf::Image& generated_image) {
    const auto original_image_size = original_image.getSize();
    const auto generated_image_size = generated_image.getSize();
    if (original_image_size != generated_image_size)
        throw std::runtime_error("Generated image size should be the same as original");

    double score = 0.0;
    for (int x; x < original_image_size.x; ++x) {
        for (int y; y < original_image_size.y; ++y) {
            // TODO write pixel comparator
            if (original_image.getPixel(x, y) == generated_image.getPixel(x, y))
                score += points_per_exact_match;
        }
    }
    return score / (original_image_size.x * original_image_size.y);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(500, 500), "Genetic Image");

    sf::Image original_image;
    if (!original_image.loadFromFile("res/lena.png"))
        throw std::runtime_error("error loading original image");

    auto original_image_size = original_image.getSize();
    ImageComposite generated_image = generateRandomImage(10, 100, original_image_size.x, original_image_size.y);
    sf::Sprite displayedSprite(generated_image.getTexture());

    std::cout << "score: " << evaluate(original_image, generated_image.getImage()) << std::endl;

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