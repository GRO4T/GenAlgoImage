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

    auto original_image_size = original_image.getSize();
    GeneratedImage generated_image = generateRandomImage(10, 100, original_image_size.x, original_image_size.y);
    sf::Sprite displayedSprite(generated_image.getTexture());

    ImageGenerator image_generator(original_image, 50, 10, 100);
    image_generator.init();
    image_generator.evaluate();
    image_generator.displayLastGenerationInfo();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        const auto img_num = std::rand() % 50;
        std::cout << "displaying image number: " << img_num << std::endl;
        displayedSprite.setTexture(image_generator.images[img_num].getTexture());

        window.clear();
        window.draw(displayedSprite);
        window.display();
    }
    return 0;
}