//
// Created by DamianPC on 7/29/2020.
//

#ifndef UNTITLED_ARASH_PARTOW_BITMAP_H
#define UNTITLED_ARASH_PARTOW_BITMAP_H

#include "../header_libs//bitmap_image.hpp"
#include "image.h"

namespace gen_algo_image{
    class ArashPartowBitmap : public Image{
    public:
        ArashPartowBitmap() : drawer(image) { drawer.pen_width(512); }
        void Create(unsigned int width, unsigned int height, Color imageColor) override {
            image.set_region(0, 0, width, height, imageColor.red, imageColor.green, imageColor.blue);
        }

        void Load(const std::string& filename){
            image = bitmap_image(filename);
        }

        void Save(const std::string& filename){
            image.save_image(filename);
        }

        bool IsPointInBounds(int x, int y) override {
            return Image::IsPointInBounds(x, y);
        }

        void DrawPixel(int x, int y, Color color) override {
            image.set_pixel(x, y, color.red, color.green, color.blue);
        }

        Color GetPixelColor(int x, int y) override {
            Color color = Color();
            image.get_pixel(x, y, color.red, color.green, color.blue);
            return color;
        }

        unsigned int GetWidth() const override {
            return image.width();
        }

        unsigned int GetHeight() const override {
            return image.height();
        }

        void ClearColor(Color color) override {
            Image::ClearColor(color);
        }

        void DrawSquare(int posx, int posy, unsigned int width, unsigned int height, Color color) override {
            image.set_region(posx, posy, width, height, color.red, color.green, color.blue);
        }

        void DrawSquare(Square square, Color color) override {
            ArashPartowBitmap::DrawSquare(square.x, square.y, square.width, square.height, color);
        }

        void DrawCircle(int posx, int posy, int radius, Color color) override {
            drawer.pen_color(color.red, color.green, color.blue);
            while (radius >= 0){
                drawer.circle(posx, posy, radius--);
            }
        }

        void DrawCircle(Circle circle, Color color) override {
            ArashPartowBitmap::DrawCircle(circle.x, circle.y, circle.radius, color);
        }

        void LoadToSFImage(sf::Image &img) override {
            image.save_image("../res/temp.bmp");
            img.loadFromFile("../res/temp.bmp");
        }

        Square GetImageBounds() override {
            return Image::GetImageBounds();
        }

        unsigned char * Data() { return image.data(); }

    private:
        bitmap_image image;
        image_drawer drawer;
    };
}

#endif //UNTITLED_ARASH_PARTOW_BITMAP_H
