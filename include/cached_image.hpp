#pragma once

#include <SFML/Graphics.hpp>

namespace gro4t {

class CachedImage {
public:
    CachedImage() {}
    CachedImage(const sf::Image& image): image(image) {
        for (int y = 0; y < image.getSize().y; ++y) {
            std::vector<sf::Color> row;
            for (int x = 0; x < image.getSize().x; ++x) {
                row.push_back(image.getPixel(x, y));
            }
            cached_pixels.push_back(row);
        }
    }

    const sf::Color& getPixel(int x, int y) const {
        return cached_pixels.at(y).at(x);
    }

    sf::Vector2u getSize() const {
        return image.getSize();
    }
private:
    sf::Image image;
    std::vector<std::vector<sf::Color>> cached_pixels;
};

}