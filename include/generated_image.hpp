#pragma once

#include "utils.hpp"
#include <cassert>

namespace gro4t {

class GeneratedImage {
public:
    GeneratedImage(const GeneratedImageProps& image_props)
        : id(next_id++),
          fitness_score(0.0),
          image(std::nullopt),
          render_texture(nullptr),
          image_props(image_props),
          dirty(true) {
        addCircle();
    }

    GeneratedImage(const GeneratedImage& other) {
        *this = other;
    }

    GeneratedImage& operator=(const GeneratedImage& other) {
        id = other.id;
        image_props = other.image_props;
        circle_prop_list = other.circle_prop_list;
        render_texture = nullptr;
        image = std::nullopt;
        fitness_score = other.fitness_score;
        dirty = true;
        return *this;
    }

    void addCircle() {
        circle_prop_list.push_back(CircleProps(image_props));
        dirty = true;
    }

    void evaluate(const sf::Image original_image) {
        if (!dirty) return;
        const auto original_image_size = original_image.getSize();
        const auto generated_image_size = getImage().getSize();
        if (original_image_size != generated_image_size)
            throw std::runtime_error("Generated image size should be the same as original");
        double score = 0.0;
        for (int x = 0; x < original_image_size.x; ++x) {
            for (int y = 0; y < original_image_size.y; ++y) {
                const auto original_pixel = original_image.getPixel(x, y);
                const auto generated_pixel = getImage().getPixel(x, y);
                score += calculatePartialMatchPoints(distance(original_pixel, generated_pixel));
            }
        }
        fitness_score = score / (original_image_size.x * original_image_size.y);
    }

    void mutate(int circle_index) {
        clear();
        const double r = real_dist(generator);
        if (r < circle_mutation_rate)
            circle_prop_list.at(circle_index).mutate();
    }

    void clear() {
        image = std::nullopt;
        dirty = true;
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
        if (dirty) generateRenderTexture();
        dirty = false;
        return *render_texture;
    }

private:
    static int next_id;
    static constexpr double circle_mutation_rate = 1;

    static const int int_dist_range = 1000;
    static std::mt19937 generator;
    static std::uniform_real_distribution<double> real_dist;
    static std::uniform_int_distribution<int> int_dist;

    bool dirty;  // tells if image needs to be redrawn
    int id;
    std::vector<CircleProps> circle_prop_list;
    RenderTexturePtr render_texture;
    std::optional<sf::Image> image;
    double fitness_score;
    GeneratedImageProps image_props;

    double calculatePartialMatchPoints(const sf::Color distance) {
        int sum = distance.r + distance.g + distance.b;
        const double max_sum = 3 * 255;
        return (max_sum - sum) / max_sum;
    }

    void generateRenderTexture() {
        if (!render_texture)
            render_texture = std::make_unique<sf::RenderTexture>();
        if (!render_texture->create(image_props.width, image_props.height))
            throw std::runtime_error("error creating render texture");
        render_texture->clear();
        for (const auto& circle_prop : circle_prop_list) {
            sf::CircleShape circle(circle_prop.radius);
            circle.setPosition(circle_prop.position);
            circle.setFillColor(circle_prop.color);
            render_texture->draw(circle);
        }
        render_texture->display();
    }
};

}  // namespace gro4t