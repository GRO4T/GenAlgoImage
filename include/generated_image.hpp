#pragma once

#include <cassert>

#include "utils.hpp"

namespace gro4t {

class GeneratedImage {
public:
    GeneratedImage(const GeneratedImageProps& image_props);
    GeneratedImage(const GeneratedImage& other);
    GeneratedImage& operator=(const GeneratedImage& other);

    std::size_t getCirclesNum() const { return circle_prop_list.size(); }
    void addCircle();
    void evaluate(const sf::Image original_image);
    void mutate(int circle_index, double sigma);
    void clear();

    int getId() const;
    const sf::Texture& getTexture();
    const sf::Image& getImage();
    double getFitnessScore() const;
    const sf::RenderTexture& getRenderTexture();

private:
    static int next_id;

    static std::mt19937 generator;
    static std::normal_distribution<double> normal_dist;
    static std::uniform_int_distribution<int> uniform_int_dist;

    bool dirty;  // tells if image needs to be redrawn
    int id;
    std::vector<CircleProps> circle_prop_list;
    RenderTexturePtr render_texture;
    std::optional<sf::Image> image;
    double fitness_score;
    GeneratedImageProps image_props;

    void generateRenderTexture();
    void mutateLayer(double sigma);
    void mutatePosition(double sigma);
    void mutateColor(double sigma);
    void mutateRadius(double sigma);
};

}  // namespace gro4t