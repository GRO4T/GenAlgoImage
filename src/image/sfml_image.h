//
// Created by DamianPC on 6/27/2020.
//

#ifndef UNTITLED_SFML_IMAGE_H
#define UNTITLED_SFML_IMAGE_H

#include <SFML/Graphics/Image.hpp>
#include "image.h"

namespace gen_algo_image{
    class SFMLImage : public Image{
        friend class SFMLImageLoader;
    public:
        void Create(unsigned int width, unsigned int height, Color imageColor) override ;

        void DrawPixel(int x, int y, Color color) override;
        Color GetPixelColor(int x, int y) override;
        unsigned int GetWidth() const override;
        unsigned int GetHeight() const override;

        sf::Image& GetImage() { return data; }

        void LoadToSFImage(sf::Image &img) override;

    private:
        sf::Image data;
    };

    void SFMLImage::Create(unsigned int width, unsigned int height, Color imageColor) {
        data.create(width, height, sf::Color(imageColor.red, imageColor.green, imageColor.blue));
    }

    void SFMLImage::DrawPixel(int x, int y, Color color) {
        data.setPixel(x, y, sf::Color(color.red, color.green, color.blue));
    }

    Color SFMLImage::GetPixelColor(int x, int y) {
        sf::Color sfmlColor = data.getPixel(x, y);
        return Color(sfmlColor.r, sfmlColor.g, sfmlColor.b);
    }

    unsigned int SFMLImage::GetWidth() const {
        return data.getSize().x;
    }

    unsigned int SFMLImage::GetHeight() const {
        return data.getSize().y;
    }

    void SFMLImage::LoadToSFImage(sf::Image &img) {
        img = data;
    }

    class SFMLImageLoader : public ImageLoader{
    public:
        void Load(Image &image, std::string filename) override {
            SFMLImage& sfmlImage = (SFMLImage&)image;
            this->filename = filename;

            sfmlImage.data.loadFromFile(filename);
        }

        void Save(Image &image, std::string filename = "") override {
            if (filename == "") filename = this->filename;
            SFMLImage& sfmlImage = (SFMLImage&)image;

            sfmlImage.data.saveToFile(filename);
        }
    private:
        std::string filename;
    };
}

#endif //UNTITLED_SFML_IMAGE_H
