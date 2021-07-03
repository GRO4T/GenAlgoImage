#pragma once

#include <ctime>

#include "utils.hpp"
#include "generated_image.hpp"

namespace gro4t {

class ImageGenerator {
public:
    ImageGenerator(const sf::Image original_image, int pop_size, int circles_num, float max_radius)
        : original_image(original_image),
          pop_size(pop_size),
          circles_num(circles_num),
          max_radius(max_radius),
          generator(time(nullptr)),
          real_dist(0.0, 1.0),
          int_dist(0, 255) {}

    void init() {
        const auto original_image_size = original_image.getSize();
        const auto img_width = original_image_size.x;
        const auto img_height = original_image_size.y;
        for (int i = 0; i < pop_size; ++i) {
            auto image_info = getRandomCircleList(circles_num, img_width, img_height, max_radius);
            images.push_back(GeneratedImage(image_info, img_width, img_height));
        }
    }

    void evaluation() {
        for (auto& img : images) {
            img.evaluate(original_image);
        }
    }

    void selection() {
    }

    void displayLastGenerationInfo() {
        for (const auto& img : images) {
            std::cout << "Image with id: " << img.getId() << std::endl;
            std::cout << "\tscore: " << img.getFitnessScore() << std::endl;
        }
    }

    GeneratedImage& getBest() {
        return *std::max_element(images.begin(), images.end(), [](const GeneratedImage& a, const GeneratedImage& b) {
            return a.getFitnessScore() < b.getFitnessScore();
        });
    }

private:
    std::vector<GeneratedImage> images;
    int pop_size;
    int circles_num;
    int max_radius;
    sf::Image original_image;

    std::mt19937 generator;
    std::uniform_real_distribution<double> real_dist;
    std::uniform_int_distribution<int> int_dist;

    std::vector<CircleProp> getRandomCircleList(int n, int image_width, int image_height,
                                                float max_radius);
};

}  // namespace gro4t