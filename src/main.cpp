#include "genetic_algorithm/genetic_algorithm.h"

#include <SFML/Graphics.hpp>
#include <thread>
#include <atomic>
#include <iostream>
#include <cstddef>

using gen_algo_image::Bitmap;
using gen_algo_image::BitmapLoader;
using gen_algo_image::SFML_ImageWrapper;
using gen_algo_image::SFMLImageLoader;
using gen_algo_image::GeneticAlgorithm;
using gen_algo_image::Individual;
using gen_algo_image::Color;
using gen_algo_image::Square;
using gen_algo_image::Circle;
using gen_algo_image::Timer;
using gen_algo_image::ArashPartowBitmapWrapper;

std::atomic<bool> ready = false;
std::atomic<bool> windowOpen = true;
sf::Image img;

template<class ImageType>
void RunGeneticAlgorithm(ImageType* original, unsigned popSize,
                         unsigned numSquares, unsigned numCircles);
void Display();

int main() {
    srand (time(NULL));

    std::string bmpFilename = "../res/lena.bmp";
    std::string sfmlImageFilename = "../res/lena.png";

    SFML_ImageWrapper original2;
    SFMLImageLoader loader2;
    loader2.Load(original2, sfmlImageFilename);

    BitmapLoader loader;
    Bitmap original;
    loader.Load(original, bmpFilename);

    ArashPartowBitmapWrapper original3;
    original3.Load(bmpFilename);

    std::thread first(RunGeneticAlgorithm<ArashPartowBitmapWrapper>, &original3, 50, 0, 50);
    Display();
    first.join();

    return 0;
}

void Display(){
    sf::Texture text;
    sf::Sprite sprite;

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML ");
    window.setFramerateLimit(30);

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
            if (event.type == sf::Event::Closed){
                window.close();
                windowOpen = false;
            }
        }
    }
}

template<class ImageType>
void RunGeneticAlgorithm(ImageType* original, unsigned popSize,
                         unsigned numSquares, unsigned numCircles){
    GeneticAlgorithm<ImageType> geneticAlgorithm(numSquares,  numCircles, original);
    Individual<ImageType> best;

    geneticAlgorithm.CreatePopulation();

    for (int i = 0; i < 20; i++){
        geneticAlgorithm.NextGeneration();
        best = geneticAlgorithm.GetBestIndividual();
        best.LoadResultToSFImage(img);
        ready = true;

        while (ready){
            if (!windowOpen) return;
        }
    }
}
