#include "figures.h"
#include "bitmap.h"
#include "sfml_image.h"

#include <SFML/Graphics.hpp>

void testBitmap(sf::Image& img, std::string filename);
void testSFMLImage(sf::Image& img, std::string filename);

int main() {
    std::string bmpFilename = "../res/lena.bmp";
    std::string sfmlImageFilename = "../res/lena.png";
    sf::Image img;

    //testBitmap(img, bmpFilename);
    testSFMLImage(img, sfmlImageFilename);

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

void testBitmap(sf::Image& img, std::string filename){
    BitmapLoader loader;
    Bitmap bmp;
    loader.load(bmp, filename);

    Square sq;
    for (int i = 0; i < 5; i++){
        sq.randomize(Square(0, 0, 512, 512));
        std::cout << "x: " << sq.x << std::endl;
        std::cout << "y: " << sq.y << std::endl;
        std::cout << "width: " << sq.width << std::endl;
        std::cout << "height: " << sq.height << std::endl;
        bmp.drawSquare(sq, Color());
        //bmp.drawCircle(sq.x, sq.y, 10.0f, Color());
    }

    bmp.clearColor(Color(200, 200, 200));
    Color color = bmp.getPixelColor(40, 40);

    std::cout << color << std::endl;

    Color color1(255, 255,255);

    std::cout << color + color1 << std::endl;
    std::cout << color - color1 << std::endl;
    std::cout << color1 - color << std::endl;

    img.loadFromMemory(bmp.data, bmp.size);

    loader.save(bmp, "../res/test_lena.bmp");
}

void testSFMLImage(sf::Image& img, std::string filename){
    SFMLImageLoader loader;
    SFMLImage sfmlImage;

    loader.load(sfmlImage, filename);

    Square sq;
    for (int i = 0; i < 5; i++){
        sq.randomize(Square(0, 0, 512, 512));
        std::cout << "x: " << sq.x << std::endl;
        std::cout << "y: " << sq.y << std::endl;
        std::cout << "width: " << sq.width << std::endl;
        std::cout << "height: " << sq.height << std::endl;
        sfmlImage.drawSquare(sq, Color());
        //bmp.drawCircle(sq.x, sq.y, 10.0f, Color());
    }

    img = sfmlImage.getImage();

    loader.save(sfmlImage, "../res/test_lena.png");
}


