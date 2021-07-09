
#include <unistd.h>

#include <iostream>
#include <memory>
#include <stdexcept>

#include "image_generator.hpp"
#include "config_loader.hpp"

using namespace gro4t;


int main() {

    sf::Sprite displayedSprite;
    ImageGenerator image_generator(ConfigLoader::loadConfig("res/image_generator.conf"));
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