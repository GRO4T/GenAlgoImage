#pragma once

#include "cached_image.hpp"

namespace gro4t {

class ImageGeneratorConfig {
public:
    int display_info_frequency;
    int next_circle_frequency;
    int sigma_evaluation_frequency;
    GeneratedImageProps image_props;
    double base_sigma;
    double expected_improvement;

    const CachedImage& getOriginalImage() const { return original_image; }
    void setOriginalImage(const sf::Image& original_image) {
        this->original_image = CachedImage(original_image);
    }

private:
    CachedImage original_image;
};

}