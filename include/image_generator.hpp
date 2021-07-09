#pragma once

#include <ctime>
#include <queue>

#include "generated_image.hpp"
#include "utils.hpp"

namespace gro4t {

struct ImageGeneratorConfig {
    int display_info_frequency;
    int next_circle_frequency;
    int sigma_evaluation_frequency;
    GeneratedImageProps image_props;
    sf::Image original_image;
};

struct State {
    State(const ImageGeneratorConfig& config)
        : config(config),
          generation(0),
          current_circle_progress(0),
          current_circle(0),
          generated_image(config.image_props),
          last_sigma_evaluation(config.sigma_evaluation_frequency) {}
    int generation;
    int current_circle_progress;
    int current_circle;
    ImageGeneratorConfig config;
    double sigma = 1.0;
    std::vector<bool> result_table;
    int last_sigma_evaluation = 10;
    GeneratedImage generated_image;

    bool nextCircle() { return current_circle_progress == config.next_circle_frequency; }
    void nextGeneration() {
        ++generation;
        ++current_circle_progress;
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