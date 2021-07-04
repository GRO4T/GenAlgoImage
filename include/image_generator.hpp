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

    bool nextCircle() {
        return current_circle_generation == config.generations_per_circle;
    }

    void nextGeneration() {
        ++generation;
        ++current_circle_generation;
    }
};


class ImageGenerator {
public:
    ImageGenerator(const ImageGeneratorConfig& config)
        : config(config), state(config), generator(std::random_device{}()), real_dist(0.0, 1.0) {}

    void init() {
        for (int i = 0; i < config.pop_size; ++i) {
            images.push_back(GeneratedImage(config.image_props));
        }
        evaluation();
        displayLastGenerationInfo();
    }

    void nextGeneration() {
        state.nextGeneration();
        if (state.current_circle_generation == config.generations_per_circle) {
            state.current_circle_generation = 0;
            state.current_circle++;
            for (auto& img : images) img.addCircle();
            evaluation();
        }
        mutation();
        selection();
        evaluation();
        displayLastGenerationInfo();
    }

    std::vector<GeneratedImage> getGeneration() { return images; }

    GeneratedImage& getBest() {
        if (state.best.has_value()) return state.best.value();
        updateBest();
        return state.best.value();
    }

    void updateBest() {
        state.best = *std::max_element(images.begin(), images.end(),
                                 [](const GeneratedImage& a, const GeneratedImage& b) {
                                     return a.getFitnessScore() < b.getFitnessScore();
                                 });
    }

private:
    State state;
    ImageGeneratorConfig config;

    std::vector<GeneratedImage> images;

    std::mt19937 generator;
    std::uniform_real_distribution<double> real_dist;

    void mutation() {
        for (auto& img : images) {
            const auto r = real_dist(generator);
            if (r < config.mutation_rate) img.mutate(state.current_circle);
        }
    }

    void evaluation() {
        for (auto& img : images) {
            img.evaluate(config.original_image);
        }
        updateBest();
    }

    void selection() {
        std::vector<GeneratedImage> new_images;
        new_images.push_back(getBest());
        std::sort(images.begin(), images.end(),
                  [](const GeneratedImage& a, const GeneratedImage& b) {
                      return a.getFitnessScore() < b.getFitnessScore();
                  });
        for (int i = 0; i < int(config.pop_size * config.percent_worst); ++i) {
            new_images.push_back(images[i]);
        }
        while (new_images.size() < config.pop_size) {
            std::vector<GeneratedImage> warriors;
            std::sample(images.begin(), images.end(), std::back_inserter(warriors), 2, generator);
            if (warriors[0].getFitnessScore() >= warriors[1].getFitnessScore())
                new_images.push_back(warriors[0]);
            else
                new_images.push_back(warriors[1]);
        }
        images = new_images;
    }

    void displayLastGenerationInfo() {
        auto& best = getBest();
        std::cout << "generation: " << state.generation << std::endl;
        std::cout << "best id: " << best.getId() << " score: " << best.getFitnessScore()
                  << std::endl;
    }
};

}  // namespace gro4t