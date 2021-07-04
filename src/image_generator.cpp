#include "image_generator.hpp"

namespace gro4t {

ImageGenerator::ImageGenerator(const ImageGeneratorConfig& config)
    : config(config), state(config), generator(std::random_device{}()), real_dist(0.0, 1.0) {}

void ImageGenerator::nextGeneration() {
    state.nextGeneration();
    if (state.nextCircle()) {
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

GeneratedImage& ImageGenerator::getBest() {
    if (state.best.has_value()) return state.best.value();
    updateBest();
    return state.best.value();
}

void ImageGenerator::updateBest() {
    state.best = *std::max_element(images.begin(), images.end(),
                                   [](const GeneratedImage& a, const GeneratedImage& b) {
                                     return a.getFitnessScore() < b.getFitnessScore();
                                   });
}

void ImageGenerator::init() {
    for (int i = 0; i < config.pop_size; ++i) {
        images.push_back(GeneratedImage(config.image_props));
    }
    evaluation();
    displayLastGenerationInfo();
}

void ImageGenerator::mutation() {
    for (auto& img : images) {
        const auto r = real_dist(generator);
        if (r < config.mutation_rate) img.mutate(state.current_circle);
    }
}

void ImageGenerator::evaluation() {
    for (auto& img : images) {
        img.evaluate(config.original_image);
    }
    updateBest();
}

void ImageGenerator::selection() {
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

void ImageGenerator::displayLastGenerationInfo() {
    auto& best = getBest();
    std::cout << "generation: " << state.generation << std::endl;
    std::cout << "best id: " << best.getId() << " score: " << best.getFitnessScore()
              << std::endl;
}

}