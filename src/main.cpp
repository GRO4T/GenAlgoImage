
#include <unistd.h>

#include <iostream>
#include <memory>
#include <stdexcept>

#include "image_generator.hpp"
#include "config_loader.hpp"

using namespace gro4t;

bool parseArgs(int argc, char ** argv, std::string& input_file, std::string& output_file);

int main(int argc, char ** argv) {
    std::string input_file;
    std::string output_file;
    if (!parseArgs(argc, argv, input_file, output_file)) return 1;
    ImageGenerator image_generator(ConfigLoader::loadConfig("res/image_generator.conf"));
    if (input_file != "")
        image_generator.loadStateFromJSON(input_file);

    sf::RenderWindow window(sf::VideoMode(500, 500), "Genetic Image");
    sf::Sprite displayedSprite;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                if (output_file != "")
                    image_generator.saveStateToJSON(output_file);
                window.close();
            }
        }
        displayedSprite.setTexture(image_generator.getGeneratedImage().getTexture());
        window.clear(sf::Color::Black);
        window.draw(displayedSprite);
        window.display();
        image_generator.nextGeneration();
    }
    return 0;
}

bool parseArgs(int argc, char ** argv, std::string& input_file, std::string& output_file) {
    std::string usage = "Usage: ./genetic_image [-h] [-i filename] [-o filename]\n"
                        "-h help\n"
                        "-i path to JSON file with initial state\n"
                        "-o path where final state will be saved\n";
    for (int i = 1; i < argc; ++i) {
        std::string arg = std::string(argv[i]);
        if (arg == "-h") {
            std::cout << usage;
            return false;
        }
        else if (arg == "-i" || arg == "-o") {
            if (++i < argc) {
                // first character of filename cannot be '-'
                if (*argv[i] == '-') {
                    std::cout << usage;
                    return false;
                }
                if (arg == "-i")
                    input_file = std::string(argv[i]);
                else
                    output_file = std::string(argv[i]);
            }
            else {
                std::cout << usage;
                return false;
            }
        }
    }
    return true;
}
