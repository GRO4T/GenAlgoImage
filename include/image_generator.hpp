#pragma once

#include "utils.hpp"

namespace gro4t {
    class ImageGenerator {
    public:
        ImageGenerator(const sf::Image original_image, int pop_size, int circles_num, float max_radius):
            original_image(original_image),
            pop_size(pop_size),
            circles_num(circles_num),
            max_radius(max_radius) {}

        void init() {
            for (int i = 0; i < pop_size; ++i) {
                images.push_back(
                    generateRandomImage(
                        circles_num, max_radius, original_image.getSize().x, original_image.getSize().y));
            }
        }

        void evaluate() {
            for (auto& img : images) {
                img.evaluate(original_image);
            }
        }

        void displayLastGenerationInfo() {
            for (const auto& img : images) {
                std::cout << "Image with id: " << img.getId() << std::endl;
                std::cout << "\tscore: " << img.getFitnessScore() << std::endl;
            }
        }
      std::vector<GeneratedImage> images;
     private:
        int pop_size;
        int circles_num;
        int max_radius;
        sf::Image original_image;
    };
}