#include "image_generator.hpp"

namespace gro4t {

ImageGenerator::ImageGenerator(const ImageGeneratorConfig& config)
    : config(config), state(config) {}

void ImageGenerator::nextGeneration() {
    state.nextGeneration();
    nextCircleIfImprovementNotSatisfactory();
    mutateAndEvaluate();
    evaluateSigma();
    if (state.generation % config.display_info_frequency == 0)
        displayLastGenerationInfo();
}

GeneratedImage& ImageGenerator::getGeneratedImage() { return state.generated_image; }

void ImageGenerator::displayLastGenerationInfo() {
    std::cout << "generation: " << state.generation << std::endl;
    std::cout << "  score: " << getGeneratedImage().getFitnessScore() << std::endl;
    std::cout << "  current_circle: " << state.current_circle << std::endl;
    std::cout << "  sigma: " << state.sigma << std::endl;
//    int successes = std::count(state.result_table.begin(), state.result_table.end(), true);
//    int tries = state.result_table.size();
//    std::cout << "  success_rate: " << (tries > 0 ? successes / (double) tries : 0.0) << std::endl;
}

void ImageGenerator::loadStateFromJSON(const std::string path) {
    state.loadFromJSON(path);
}

void ImageGenerator::saveStateToJSON(const std::string path) {
    state.saveToJSON(path);
}
void ImageGenerator::nextCircleIfImprovementNotSatisfactory() {
    if (state.nextCircle()) {
        state.current_circle_progress = 0;
        if (state.generated_image.getFitnessScore() - state.saved_fitness < config.expected_improvement) {
            state.current_circle = (state.current_circle + 1) % config.image_props.max_circles;
            state.sigma = config.base_sigma;
        }
        state.saved_fitness = state.generated_image.getFitnessScore();
    }
}

void ImageGenerator::mutateAndEvaluate() {
    auto new_generated_image = getGeneratedImage();
    if (new_generated_image.getCirclesNum() < config.image_props.max_circles &&
        new_generated_image.getCirclesNum() <= state.current_circle)
        new_generated_image.addCircle();
    else
        new_generated_image.mutate(state.current_circle, state.sigma);

    new_generated_image.evaluate(config.getOriginalImage());
    if (new_generated_image.getFitnessScore() > getGeneratedImage().getFitnessScore()) {
        state.generated_image = new_generated_image;
        state.result_table.push_back(true);
    } else
        state.result_table.push_back(false);
}

void ImageGenerator::evaluateSigma() {
    if (--state.last_sigma_evaluation == 0) {
        state.last_sigma_evaluation = config.sigma_evaluation_frequency;
        int successes = std::count(state.result_table.begin(), state.result_table.end(), true);
        state.result_table.clear();
        double success_rate = successes / (double)config.sigma_evaluation_frequency;
        if (success_rate < 0.2) state.sigma = 1.22 * state.sigma;
        if (success_rate > 0.2) state.sigma = 0.82 * state.sigma;
    }
}

}  // namespace gro4t