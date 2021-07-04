#include "generated_image.hpp"

namespace gro4t {

int GeneratedImage::next_id = 0;
std::mt19937 GeneratedImage::generator(std::random_device{}());
std::uniform_real_distribution<double> GeneratedImage::real_dist(0.0, 1.0);

double calculatePartialMatchPoints(const sf::Color distance) {
    int sum = distance.r + distance.g + distance.b;
    const double max_sum = 3 * 255;
    return (max_sum - sum) / max_sum;
}

GeneratedImage::GeneratedImage(const GeneratedImageProps& image_props)
    : id(next_id++),
      fitness_score(0.0),
      image(std::nullopt),
      render_texture(nullptr),
      image_props(image_props),
      dirty(true) {
    addCircle();
}

GeneratedImage::GeneratedImage(const GeneratedImage& other) { *this = other; }

GeneratedImage& GeneratedImage::operator=(const GeneratedImage& other) {
    id = next_id++;
    image_props = other.image_props;
    circle_prop_list = other.circle_prop_list;
    render_texture = nullptr;
    image = std::nullopt;
    fitness_score = other.fitness_score;
    dirty = true;
    return *this;
}

void GeneratedImage::addCircle() {
    circle_prop_list.push_back(CircleProps(image_props));
    dirty = true;
}

void GeneratedImage::evaluate(const sf::Image original_image) {
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

void GeneratedImage::mutate(int circle_index) {
    clear();
    const double r = real_dist(generator);
    if (r < circle_mutation_rate) circle_prop_list.at(circle_index).mutate();
}

void GeneratedImage::clear() {
    render_texture = nullptr;
    image = std::nullopt;
    dirty = true;
}

void GeneratedImage::generateRenderTexture() {
    if (!render_texture) render_texture = std::make_unique<sf::RenderTexture>();
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

int GeneratedImage::getId() const { return id; }

const sf::Texture& GeneratedImage::getTexture() { return getRenderTexture().getTexture(); }

const sf::Image& GeneratedImage::getImage() {
    if (image.has_value()) return image.value();
    image = getTexture().copyToImage();
    return image.value();
}

double GeneratedImage::getFitnessScore() const { return fitness_score; }

const sf::RenderTexture& GeneratedImage::getRenderTexture() {
    if (dirty) generateRenderTexture();
    dirty = false;
    return *render_texture;
}

}  // namespace gro4t
