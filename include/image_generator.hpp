#pragma once

#include <ctime>
#include <queue>

#include "generated_image.hpp"
#include "utils.hpp"

namespace gro4t {

struct ImageGeneratorConfig {
    int generations_per_circle;
    int generations_per_evaluation;
    GeneratedImageProps image_props;
    sf::Image original_image;
};

struct State {
    State(const ImageGeneratorConfig& config)
        : config(config),
          generation(0),
          current_circle_generation(0),
          current_circle(0),
          generated_image(config.image_props),
          generations_since_last_evaluation(config.generations_per_evaluation) {}
    int generation;
    int current_circle_generation;
    int current_circle;
    ImageGeneratorConfig config;
    double sigma = 1.0;
    std::vector<bool> result_table;
    int generations_since_last_evaluation = 10;
    GeneratedImage generated_image;

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

    GeneratedImage& getGeneratedImage();

private:
    State state;
    ImageGeneratorConfig config;

    std::mt19937 generator;
    std::uniform_real_distribution<double> real_dist;

    void displayLastGenerationInfo();
};

}  // namespace gro4t