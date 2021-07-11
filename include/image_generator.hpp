#pragma once

#include <ctime>
#include <queue>

#include "generated_image.hpp"
#include "utils.hpp"
#include "state.hpp"

namespace gro4t {


class ImageGenerator {
public:
    explicit ImageGenerator(const ImageGeneratorConfig& config);
    ~ImageGenerator() {}

    void nextGeneration();

    GeneratedImage& getGeneratedImage();
    int getGeneration() const { return state.generation; }
    int getImageWidth() const { return state.config.image_props.width; }
    int getImageHeight() const { return state.config.image_props.height; }

    void loadStateFromJSON(const std::string path);
    void saveStateToJSON(const std::string path);

private:
    State state;
    ImageGeneratorConfig config;

    void displayLastGenerationInfo();
    void nextCircleIfImprovementNotSatisfactory();
    void mutateAndEvaluate();
    void evaluateSigma();
};

}  // namespace gro4t