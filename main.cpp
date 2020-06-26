#include <iostream>
#include <sstream>

#include "bitmap_loader.h"
#include "figures.h"

#include <SFML/Graphics.hpp>

int main() {
    std::string filename = "../res/lena.bmp";
    BitmapLoader loader;
    Bitmap bmp;
    loader.load(bmp, filename);

    Square sq;
    for (int i = 0; i < 5; i++){
        sq.randomize(*new Square(0, 0, 512, 512));
        std::cout << "posx: " << sq.posx << std::endl;
        std::cout << "posy: " << sq.posy << std::endl;
        std::cout << "width: " << sq.width << std::endl;
        std::cout << "height: " << sq.height << std::endl;
        bmp.drawSquare(sq, *new Color());
    }

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

    loader.save(bmp, "../res/test.bmp");
    return 0;
}


