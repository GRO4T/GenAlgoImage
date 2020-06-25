//
// Created by DamianPC on 6/27/2020.
//

#ifndef UNTITLED_SFML_IMAGE_WRAPPER_H
#define UNTITLED_SFML_IMAGE_WRAPPER_H

#include <SFML/Graphics/Image.hpp>
#include "image.h"

namespace gen_algo_image {
    class SFML_ImageWrapper : public Image {
        friend class SFMLImageLoader;

    public:
        void Create(unsigned int width, unsigned int height, Color imageColor) override;

        void DrawPixel(int x, int y, Color color) override;

        Color GetPixelColor(int x, int y) override;

        unsigned int GetWidth() const override;

        unsigned int GetHeight() const override;

        sf::Image &GetImage() { return data; }

        void LoadToSFImage(sf::Image &img) override;

        void GetPixelColor(const int &x, const int &y, Color &color) override;

    private:
        sf::Image data;
    };


    class SFMLImageLoader : public ImageLoader {
    public:
        inline void Load(Image &image, std::string filename) override {
            SFML_ImageWrapper &sfmlImage = (SFML_ImageWrapper &) image;
            this->filename = filename;

            sfmlImage.data.loadFromFile(filename);
        }

        inline void Save(Image &image, std::string filename = "") override {
            if (filename == "") filename = this->filename;
            SFML_ImageWrapper &sfmlImage = (SFML_ImageWrapper &) image;

            sfmlImage.data.saveToFile(filename);
        }

    private:
        std::string filename;
    };
}

#endif //UNTITLED_SFML_IMAGE_WRAPPER_H
