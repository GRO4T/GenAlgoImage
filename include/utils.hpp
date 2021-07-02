#pragma once

#include <SFML/Graphics.hpp>
#include <random>
#include <ctime>
#include <cmath>
#include <iostream>

namespace gro4t {
    using RenderTexturePtr = std::unique_ptr<sf::RenderTexture>;

    inline sf::Color distance(const sf::Color& color_a, const sf::Color& color_b) {
        auto color_distance = [](uint8_t a, uint8_t b) {
            const auto diff = std::abs(a - b);
            return (uint8_t) std::min(diff, 255 - diff);
        };
        const auto diff_r = color_distance(color_a.r, color_b.r);
        const auto diff_g = color_distance(color_a.g, color_b.g);
        const auto diff_b = color_distance(color_a.b, color_b.b);
        return { diff_r, diff_g, diff_b };
    }

    inline std::ostream& operator<<(std::ostream& os, const sf::Color& color) {
        os << "(" << (int) color.r << ", " << (int) color.g << ", " << (int) color.b << ")";
        return os;
    }

    struct CircleProp {
        float radius;
        sf::Vector2f position;
        sf::Color color;
    };

    class GeneratedImage {
    public:
        GeneratedImage(RenderTexturePtr &renderTexture, const sf::Image &image) : renderTexture(std::move(renderTexture)),
                                                                                  image(image),
                                                                                  id(next_id++) {}

        void evaluate(const sf::Image original_image) {
            const auto original_image_size = original_image.getSize();
            const auto generated_image_size = image.getSize();
            if (original_image_size != generated_image_size)
                throw std::runtime_error("Generated image size should be the same as original"); double score = 0.0;
            for (int x = 0; x < original_image_size.x; ++x) {
                for (int y = 0; y < original_image_size.y; ++y) {
                    const auto original_pixel = original_image.getPixel(x, y);
                    const auto generated_pixel = image.getPixel(x, y);
                    if (original_pixel == generated_pixel)
                        score += points_for_exact_match;
                    score += calculatePartialMatchPoints(distance(original_pixel, generated_pixel));
                }
            }
            fitness_score = score / (original_image_size.x * original_image_size.y);
        }

        int getId() const { return id; }
        const sf::Texture& getTexture() const { return renderTexture->getTexture(); }
        const sf::Image& getImage() const { return image; }
        double getFitnessScore() const { return fitness_score; }
    private:
        const double points_for_exact_match = 1.0;
        const double points_for_partial_match = 1.0;
        static int next_id;

        double calculatePartialMatchPoints(const sf::Color distance) {
            const auto sum = distance.r + distance.g + distance.b;
            const double max_sum = 3 * 255;
            return points_for_partial_match * (max_sum - sum) / max_sum;
        }

        int id;
        RenderTexturePtr renderTexture;
        sf::Image image;
        double fitness_score;
    };

    std::vector<CircleProp> getRandomCircleList(int n, int image_width, int image_height, float max_radius);
    GeneratedImage generateRandomImage(int circles_num, float max_radius, int image_width, int image_height);
}