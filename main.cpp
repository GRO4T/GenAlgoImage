#include <iostream>
#include <sstream>

#include "bitmap_loader.h"

#include <SFML/Graphics.hpp>

int main() {
    std::cout << "Hello, World!" << std::endl;

    std::string filename = "../lena.bmp";

    BitmapLoader loader;
    Bitmap bmp;
    loader.load(bmp, filename);
    sf::Image img;
    img.loadFromMemory(bmp.data, bmp.rawDataSize);
/*
    sf::Texture text;
    text.loadFromImage(img);

    sf::Sprite sprite;
    sprite.setTexture(text);

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML ");

    while (window.isOpen()){
        window.clear();
        window.draw(sprite);
        window.display();
    }
*/
    return 0;
}


