
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

    GeneratedImageProps image_props(100, original_image);
    ImageGeneratorConfig config{1, 0.2, 100, 20, image_props, original_image};
    ImageGenerator image_generator(config);
    image_generator.init();
//    for (int i = 0; i < 10; ++i) {
//        image_generator.nextGeneration();
//    }

//    auto final_generation = image_generator.getGeneration();
//    int i = 0;

    sf::RenderWindow window(sf::VideoMode(500, 500), "Genetic Image");
    while (window.isOpen())
    {
//        sleep(1);
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
//        std::cout << "genome: " << i << std::endl;
//        displayedSprite.setTexture(final_generation[i].getTexture());
        displayedSprite.setTexture(image_generator.getBest().getTexture());
        window.clear(sf::Color::Black);
        window.draw(displayedSprite);
        window.display();
        image_generator.nextGeneration();
//        i = (i + 1) % 20;
    }
    return 0;
}