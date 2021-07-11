
#include <unistd.h>

#include <iostream>
#include <memory>
#include <stdexcept>

#include "image_generator.hpp"
#include "config_loader.hpp"

using namespace gro4t;

std::string input_file;
std::string output_file;
int generations_to_simulate = -1;
int initial_generation;

bool parseArgs(int argc, char ** argv);
void checkAppShouldClose(sf::RenderWindow& window, ImageGenerator& image_generator);

int main(int argc, char ** argv) {
    if (!parseArgs(argc, argv)) return 1;

    ImageGenerator image_generator(ConfigLoader::loadConfig("res/image_generator.conf"));
    if (input_file != "")
        image_generator.loadStateFromJSON(input_file);
    initial_generation = image_generator.getGeneration();

    sf::RenderWindow window(sf::VideoMode(512, 512), "Genetic Image");
    sf::Sprite displayedSprite;
    while (window.isOpen())
    {
        checkAppShouldClose(window, image_generator);
        displayedSprite.setTexture(image_generator.getGeneratedImage().getTexture());
        window.clear(sf::Color::Black);
        window.draw(displayedSprite);
        window.display();
        image_generator.nextGeneration();
    }
    return 0;
}

bool parseArgs(int argc, char ** argv) {
    std::string usage = "Usage: ./genetic_image [-h] [-i filename] [-o filename] [-n number]\n"
                        "-h help\n"
                        "-i path to JSON file with initial state\n"
                        "-o path where final state will be saved\n"
                        "-n number of generations to simulate\n";
    for (int i = 1; i < argc; ++i) {
        std::string arg = std::string(argv[i]);
        if (arg == "-h") {
            std::cout << usage;
            return false;
        }
        else if (arg == "-i" || arg == "-o" || arg == "-n") {
            if (++i < argc) {
                // first character of neither a filename nor a number cannot be '-'
                if (*argv[i] == '-') {
                    std::cout << usage;
                    return false;
                }
                if (arg == "-i")
                    input_file = std::string(argv[i]);
                else if (arg == "-o")
                    output_file = std::string(argv[i]);
                else
                    generations_to_simulate = std::stoi(argv[i]);
            }
            else {
                std::cout << usage;
                return false;
            }
        }
    }
    return true;
}

bool stopSimulation(const ImageGenerator& image_generator) {
    if (generations_to_simulate == -1) return false;
    return initial_generation + generations_to_simulate <= image_generator.getGeneration();
}

void closeApp(sf::RenderWindow& window, ImageGenerator& image_generator) {
    if (output_file != "")
        image_generator.saveStateToJSON(output_file);
    window.close();
}

void checkAppShouldClose(sf::RenderWindow& window, ImageGenerator& image_generator) {
    if (stopSimulation(image_generator))
        closeApp(window, image_generator);
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            closeApp(window, image_generator);
    }
}
