#include <iostream>
#include <sstream>

#include "bitmap_loader.h"

#include <SFML/Graphics.hpp>

int main() {
    std::string filename = "../lena.bmp";
    BitmapLoader loader;
    Bitmap bmp;
    loader.load(bmp, filename);

    sf::Image img;
    img.loadFromMemory(bmp.data, bmp.size);

    sf::Texture text;
    text.loadFromImage(img);

    sf::Sprite sprite;
    sprite.setTexture(text);

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML ");
    window.setFramerateLimit(30);

    while (window.isOpen()){
        window.clear();
        window.draw(sprite);
        window.display();
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
    return 0;
}


