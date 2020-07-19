#include "figures.h"
#include "image/bitmap.h"
#include "image/sfml_image.h"
#include "genetic_algorithm.h"

#include <SFML/Graphics.hpp>
#include <thread>
#include <atomic>
#include <memory>

std::atomic<bool> ready = false;
sf::Image img;

void testBitmap(sf::Image& img, std::string filename);
void testSFMLImage(sf::Image& img, std::string filename);

void testcpp17();

void testColor();


void doStuff(Bitmap* original){
    GeneticAlgorithm<Bitmap> geneticAlgorithm(0,  50,original);
    geneticAlgorithm.createPopulation(50);
    Individual<Bitmap> * best;
    for (int i = 0; i < 20; i++){
        geneticAlgorithm.nextGeneration();
        geneticAlgorithm.displayBestIndividual(5);
        best = geneticAlgorithm.getBestIndividual();
        best->loadResultToSFImage(img);
        ready = true;

        while (ready);
    }
}

void doStuff2(SFMLImage* original){
    GeneticAlgorithm<SFMLImage> geneticAlgorithm(0,  50,original);
    geneticAlgorithm.createPopulation(50);
    Individual<SFMLImage> * best;
    for (int i = 0; i < 20; i++){
        geneticAlgorithm.nextGeneration();
        geneticAlgorithm.displayBestIndividual(5);
        best = geneticAlgorithm.getBestIndividual();
        best->loadResultToSFImage(img);
        ready = true;

        while (ready);
    }
}


int main() {
    srand (time(NULL));

    std::string bmpFilename = "../res/lena.bmp";
    std::string sfmlImageFilename = "../res/lena.png";

    SFMLImage original2;
    SFMLImageLoader loader2;
    loader2.load(original2, sfmlImageFilename);

    BitmapLoader loader;
    Bitmap original;
    loader.load(original, bmpFilename);
    //original.create(512, 512, Color(0, 0, 0));

    testBitmap(img, bmpFilename);
    //testSFMLImage(img, sfmlImageFilename);
    //testcpp17();
    //testColor();

    //Individual<SFMLImage> i(2, 2, Square(0, 0, 512, 512));
    //Individual<SFMLImage> i(0, 3, Square(0, 0, 512, 512));
    //i.randomize();
    //i.loadResultToSFImage(img);
    //std::cout << "score: " << i.evaluate(original) << std::endl;


    //geneticAlgorithm.saveResultsToFiles("../res/test/test");

    sf::Texture text;
    sf::Sprite sprite;

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML ");
    window.setFramerateLimit(30);

    //std::thread first(doStuff, &original);
    std::thread first(doStuff2, &original2);


    while (window.isOpen()){
        if (ready){
            text.loadFromImage(img);
            sprite.setTexture(text);
            ready = false;
        }
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

    first.join();
    return 0;
}

void testBitmap(sf::Image& img, std::string filename){
    BitmapLoader loader;
    Bitmap bmp;
    loader.load(bmp, filename);

    Timeit stoper;
    stoper.start();
    bmp.clearColor(Color(255, 255, 255));
    stoper.stop();
    std::cout << "lena bmp clear ";
    stoper.time();

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
