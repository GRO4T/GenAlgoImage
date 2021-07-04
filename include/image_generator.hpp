#pragma once

#include <ctime>

#include "generated_image.hpp"
#include "utils.hpp"

namespace gro4t {

struct ImageGeneratorConfig {
    double mutation_rate = 1;
    double percent_worst = 0.2;
    int generations_per_circle = 100;
    int pop_size;
    GeneratedImageProps image_props;
    sf::Image original_image;
};

struct State {
    State(const ImageGeneratorConfig& config) : config(config), best(std::nullopt), generation(0), current_circle_generation(0), current_circle(0) {}
    std::optional<GeneratedImage> best;
    int generation;
    int current_circle_generation;
    int current_circle;
    ImageGeneratorConfig config;

    bool nextCircle() { return current_circle_generation == config.generations_per_circle; }
    void nextGeneration() {
        ++generation;
        ++current_circle_generation;
    }
};


class ImageGenerator {
public:
    explicit ImageGenerator(const ImageGeneratorConfig& config);

    void init();

    void nextGeneration();
    void updateBest();

    std::vector<GeneratedImage> getGeneration() { return images; }
    GeneratedImage& getBest();

private:
    State state;
    ImageGeneratorConfig config;

    std::vector<GeneratedImage> images;

    std::mt19937 generator;
    std::uniform_real_distribution<double> real_dist;

    void mutation();
    void evaluation();
    void selection();
    void displayLastGenerationInfo();
};

}  // namespace gro4t