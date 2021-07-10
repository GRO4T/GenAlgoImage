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
    void loadStateFromJSON(const std::string path);
    void saveStateToJSON(const std::string path);

private:
    State state;
    ImageGeneratorConfig config;

    void displayLastGenerationInfo();
};

}  // namespace gro4t