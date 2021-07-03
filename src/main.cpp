#include <stdexcept>
#include <vector>
#include <memory>
#include <iostream>

#include "image_generator.hpp"

using namespace gro4t;


int main() {
    sf::RenderWindow window(sf::VideoMode(500, 500), "Genetic Image");

    sf::Image original_image;
    if (!original_image.loadFromFile("res/lena.png"))
        throw std::runtime_error("error loading original image");

    sf::Sprite displayedSprite;

    ImageGenerator image_generator(original_image, 50, 10, 100);
    image_generator.init();
    image_generator.evaluation();
    image_generator.displayLastGenerationInfo();

    auto& best = image_generator.getBest();
    std::cout << "best id: " << best.getId() << " score: " << best.getFitnessScore() << std::endl;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

//        const auto img_num = std::rand() % 50;
//        std::cout << "displaying image number: " << img_num << std::endl;
        displayedSprite.setTexture(best.getTexture());

        window.clear();
        window.draw(displayedSprite);
        window.display();
    }
    return 0;
}