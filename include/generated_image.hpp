#pragma once

#include "utils.hpp"

namespace gro4t {

class GeneratedImage {
public:
    GeneratedImage(const std::vector<CircleProp>& image_props, int width, int height)
        : image_props(image_props),
          id(next_id++),
          fitness_score(0.0),
          image(std::nullopt),
          render_texture(nullptr),
          width(width),
          height(height) {}

    void evaluate(const sf::Image original_image) {
        const auto original_image_size = original_image.getSize();
        const auto generated_image_size = getImage().getSize();
        if (original_image_size != generated_image_size)
            throw std::runtime_error("Generated image size should be the same as original");
        double score = 0.0;
        for (int x = 0; x < original_image_size.x; ++x) {
            for (int y = 0; y < original_image_size.y; ++y) {
                const auto original_pixel = original_image.getPixel(x, y);
                const auto generated_pixel = getImage().getPixel(x, y);
                if (original_pixel == generated_pixel) score += points_for_exact_match;
                score += calculatePartialMatchPoints(distance(original_pixel, generated_pixel));
            }
        }
        fitness_score = score / (original_image_size.x * original_image_size.y);
    }

    int getId() const { return id; }
    const sf::Texture& getTexture() { return getRenderTexture().getTexture(); }
    const sf::Image& getImage() {
        if (image.has_value()) return image.value();
        image = getTexture().copyToImage();
        return image.value();
    }
    double getFitnessScore() const { return fitness_score; }
    const sf::RenderTexture& getRenderTexture() {
        if (render_texture == nullptr) generate();
        return *render_texture;
    }

private:
    const double points_for_exact_match = 1.0;
    const double points_for_partial_match = 1.0;
    static int next_id;

    int id;
    std::vector<CircleProp> image_props;
    int width;
    int height;
    RenderTexturePtr render_texture;
    std::optional<sf::Image> image;
    double fitness_score;

    double calculatePartialMatchPoints(const sf::Color distance) {
        const auto sum = distance.r + distance.g + distance.b;
        const double max_sum = 3 * 255;
        return points_for_partial_match * (max_sum - sum) / max_sum;
    }

    void generate() {
        render_texture = std::make_unique<sf::RenderTexture>();
        if (!render_texture->create(width, height))
            throw std::runtime_error("error creating render texture");
        render_texture->clear();
        for (const auto& circle_prop : image_props) {
            sf::CircleShape circle(circle_prop.radius);
            circle.setPosition(circle_prop.position);
            circle.setFillColor(circle_prop.color);
            render_texture->draw(circle);
        }
        render_texture->display();
    }
};

}