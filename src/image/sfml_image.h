//
// Created by DamianPC on 6/27/2020.
//

#ifndef UNTITLED_SFML_IMAGE_H
#define UNTITLED_SFML_IMAGE_H

#include <SFML/Graphics/Image.hpp>
#include "image.h"

class SFMLImage : public Image{
    friend class SFMLImageLoader;
public:
    void create(unsigned int width, unsigned int height, Color imageColor) override ;

    void drawPixel(int x, int y, Color color) override;
    Color getPixelColor(int x, int y) override;
    unsigned int getWidth() const override;
    unsigned int getHeight() const override;

    sf::Image& getImage() { return data; }

    void loadToSFImage(sf::Image &img) override;

private:
    sf::Image data;
};

void SFMLImage::create(unsigned int width, unsigned int height, Color imageColor) {
    data.create(width, height, sf::Color(imageColor.red, imageColor.green, imageColor.blue));
}

void SFMLImage::drawPixel(int x, int y, Color color) {
    data.setPixel(x, y, sf::Color(color.red, color.green, color.blue));
}

Color SFMLImage::getPixelColor(int x, int y) {
    sf::Color sfmlColor = data.getPixel(x, y);
    return Color(sfmlColor.r, sfmlColor.g, sfmlColor.b);
}

unsigned int SFMLImage::getWidth() const {
    return data.getSize().x;
}

unsigned int SFMLImage::getHeight() const {
    return data.getSize().y;
}

void SFMLImage::loadToSFImage(sf::Image &img) {
    img = data;
}

class SFMLImageLoader : public ImageLoader{
public:
    void load(Image &image, std::string filename) override {
        SFMLImage& sfmlImage = (SFMLImage&)image;
        this->filename = filename;

        sfmlImage.data.loadFromFile(filename);
    }

    void save(Image &image, std::string filename = "") override {
        if (filename == "") filename = this->filename;
        SFMLImage& sfmlImage = (SFMLImage&)image;

        sfmlImage.data.saveToFile(filename);
    }
private:
    std::string filename;
};


#endif //UNTITLED_SFML_IMAGE_H
