#pragma once

namespace gro4t {

struct ImageGeneratorConfig {
    int display_info_frequency;
    int next_circle_frequency;
    int sigma_evaluation_frequency;
    GeneratedImageProps image_props;
    sf::Image original_image;
    double base_sigma;
    double expected_improvement;
};

}