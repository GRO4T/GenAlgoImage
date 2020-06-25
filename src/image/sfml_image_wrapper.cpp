//
// Created by damian on 7/31/20.
//

#include "sfml_image_wrapper.h"

namespace gen_algo_image{
    void SFML_ImageWrapper::Create(unsigned int width, unsigned int height, Color imageColor) {
        data.create(width, height, sf::Color(imageColor.red, imageColor.green, imageColor.blue));
    }

    void SFML_ImageWrapper::DrawPixel(int x, int y, Color color) {
        data.setPixel(x, y, sf::Color(color.red, color.green, color.blue));
    }

    Color SFML_ImageWrapper::GetPixelColor(int x, int y) {
        sf::Color sfmlColor = data.getPixel(x, y);
        return Color(sfmlColor.r, sfmlColor.g, sfmlColor.b);
    }

    unsigned int SFML_ImageWrapper::GetWidth() const {
        return data.getSize().x;
    }

    unsigned int SFML_ImageWrapper::GetHeight() const {
        return data.getSize().y;
    }

    void SFML_ImageWrapper::LoadToSFImage(sf::Image &img) {
        img = data;
    }

    void SFML_ImageWrapper::GetPixelColor(const int &x, const int &y, Color &color) {
        throw std::runtime_error("not implemented");
    }
}

