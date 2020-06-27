#include "figures.h"
#include "bitmap.h"
#include "sfml_image.h"
#include "genetic_algorithm.h"

#include <SFML/Graphics.hpp>

void testBitmap(sf::Image& img, std::string filename);
void testSFMLImage(sf::Image& img, std::string filename);

void testcpp17();

void testColor();


int main() {
    srand (time(NULL));

    std::string bmpFilename = "../res/lena.bmp";
    std::string sfmlImageFilename = "../res/lena.png";
    sf::Image img;

    SFMLImage original;
    SFMLImageLoader loader;
    loader.load(original, sfmlImageFilename);
    //original.create(512, 512, Color(0, 0, 0));

    //testBitmap(img, bmpFilename);
    //testSFMLImage(img, sfmlImageFilename);
    //testcpp17();
    testColor();


    //Individual<SFMLImage> i(2, 2, Square(0, 0, 512, 512));
    Individual<SFMLImage> i(0, 0, Square(0, 0, 512, 512));
    i.randomize();
    i.loadResultToSFImage(img);
    std::cout << "score: " << i.evaluate(original) << std::endl;

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
    bmp.loadToSFImage(img);

    loader.save(bmp, "../res/test_lena.bmp");
}

void testSFMLImage(sf::Image& img, std::string filename){
    SFMLImageLoader loader;
    SFMLImage sfmlImage;

    loader.load(sfmlImage, filename);

    Circle c;
    for (int i = 0; i < 5; i++){
        c.randomize(Square(0, 0, 512, 512));
        std::cout << "x: " << c.x << std::endl;
        std::cout << "y: " << c.y << std::endl;
        std::cout << "radius: " << c.radius << std::endl;
        sfmlImage.drawCircle(c, Color());
        //bmp.drawCircle(sq.x, sq.y, 10.0f, Color());
    }

    sfmlImage.loadToSFImage(img);

    loader.save(sfmlImage, "../res/test_lena.png");
}

void testcpp17(){
    std::byte b[1];
    std::ifstream in("../res/lena.bmp", std::ios::in | std::ios::binary);
    in.read((char*)b, 1);
    std::cout << std::to_integer<int>(b[0]) << std::endl;

}

void testColor(){
    Color color(40, 40, 40);

    std::cout << color << std::endl;

    Color color1(255, 255,255);

    std::cout << color + color1 << std::endl;
    std::cout << color - color1 << std::endl;
    std::cout << color1 - color << std::endl;
    std::cout << "diff1: " << color.diff(color1) << std::endl;
    std::cout << "diff2: " << color1.diff(color) << std::endl;
}
