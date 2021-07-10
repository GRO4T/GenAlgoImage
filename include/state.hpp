#pragma once

#include "config.hpp"

namespace gro4t {

struct State {
    State(const ImageGeneratorConfig& config)
        : config(config),
          generation(0),
          current_circle_progress(0),
          current_circle(0),
          generated_image(config.image_props),
          last_sigma_evaluation(config.sigma_evaluation_frequency),
          sigma(config.base_sigma) {}
    int generation;
    int current_circle_progress;
    int current_circle;
    ImageGeneratorConfig config;
    double sigma;
    std::vector<bool> result_table;
    int last_sigma_evaluation;
    GeneratedImage generated_image;

    bool nextCircle() { return current_circle_progress == config.next_circle_frequency; }
    void nextGeneration() {
        ++generation;
        ++current_circle_progress;
    }
};

}