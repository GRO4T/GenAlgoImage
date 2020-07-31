//
// Created by DamianPC on 7/29/2020.
//

#ifndef UNTITLED_ARASH_PARTOW_BITMAP_WRAPPER_H
#define UNTITLED_ARASH_PARTOW_BITMAP_WRAPPER_H

#include "../header_libs//bitmap_image.hpp"
#include "image.h"

namespace gen_algo_image{
    class ArashPartowBitmapWrapper : public Image{
    public:
        ArashPartowBitmapWrapper();

        ArashPartowBitmapWrapper(const ArashPartowBitmapWrapper& b2);

        ArashPartowBitmapWrapper& operator=(const ArashPartowBitmapWrapper &b2);

        void Create(unsigned int width, unsigned int height, Color imageColor) override;

        void Load(const std::string &filename);

        void Save(const std::string &filename);

        bool IsPointInBounds(int x, int y) override;

        void DrawPixel(int x, int y, Color color) override;

        Color GetPixelColor(int x, int y) override;

        void GetPixelColor(const int &x, const int &y, Color &color) override;

        unsigned int GetWidth() const override;

        unsigned int GetHeight() const override;

        void ClearColor(Color color) override;

        void DrawSquare(int posx, int posy, unsigned int width, unsigned int height, Color color) override;

        void DrawSquare(Square square, Color color) override;

        void DrawCircle(const int &posx, const int &posy, int radius, const Color &color) override;

        void DrawCircle(const Circle &circle, const Color &color) override;

        void LoadToSFImage(sf::Image &img) override;

        Square GetImageBounds() override;

        inline unsigned char *Data() { return image.data(); }
    private:
        bitmap_image image;
        image_drawer drawer;
    };
}

#endif //UNTITLED_ARASH_PARTOW_BITMAP_WRAPPER_H
