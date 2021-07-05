
#include <unistd.h>

#include <iostream>
#include <memory>
#include <stdexcept>

#include "image_generator.hpp"

using namespace gro4t;


int main() {

    sf::Image original_image;
    if (!original_image.loadFromFile("res/lena.png"))
        throw std::runtime_error("error loading original image");

    sf::Sprite displayedSprite;

    GeneratedImageProps image_props(20, original_image, 400, 5);
    ImageGeneratorConfig config{500, 10, image_props, original_image};
    ImageGenerator image_generator(config);

    sf::RenderWindow window(sf::VideoMode(500, 500), "Genetic Image");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        displayedSprite.setTexture(image_generator.getGeneratedImage().getTexture());
        window.clear(sf::Color::Black);
        window.draw(displayedSprite);
        window.display();
        image_generator.nextGeneration();
    }
    return 0;
}