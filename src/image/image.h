//
// Created by DamianPC on 6/27/2020.
//

#ifndef UNTITLED_IMAGE_H
#define UNTITLED_IMAGE_H

#include <SFML/Graphics/Image.hpp>
#include "../figures.h"
namespace gen_algo_image{
    class Image{
    public:
        virtual void Create(unsigned int width, unsigned int height, Color imageColor) = 0;

        virtual bool IsPointInBounds(int x, int y) { return (x >= 0 && x < GetWidth()) && (y >= 0 && y < GetHeight()); }

        virtual void DrawPixel(int x, int y, Color color) = 0;
        virtual Color GetPixelColor(int x, int y) = 0;

        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;

        virtual void ClearColor(Color color);
        virtual void DrawSquare(int posx, int posy, unsigned int width, unsigned int height, Color color);
        virtual void DrawSquare(Square square, Color color);
        virtual void DrawCircle(int posx, int posy, int radius, Color color);
        virtual void DrawCircle(Circle circle, Color color);

        virtual void LoadToSFImage(sf::Image& img) = 0;

        virtual Square GetImageBounds();
    };

    void Image::ClearColor(Color color) {
        DrawSquare(0, 0, GetWidth(), GetHeight(), color);
    }

    void Image::DrawSquare(int posx, int posy, unsigned int width, unsigned int height, Color color) {
        for (int y = posy; y < posy + height; y++){
            for (int x = posx; x < posx + width; x++){
                DrawPixel(x, y, color);
            }
        }
    }

    void Image::DrawSquare(Square square, Color color) {
        DrawSquare(square.x, square.y, square.width, square.height, color);
    }

    void Image::DrawCircle(int posx, int posy, int radius, Color color) {
        if (radius < 0.0f)
            return;
        for (int y = posy - radius; y < posy + radius; y++){
            for (int x = posx - radius; x < posx + radius; x++){
                if (IsPointInBounds(x, y)){
                    int dx = x - posx;
                    int dy = y - posy;
                    if (dx * dx + dy * dy < radius * radius)
                        DrawPixel(x, y, color);
                }
            }
        }
    }

    void Image::DrawCircle(Circle circle, Color color) {
        DrawCircle(circle.x, circle.y, circle.radius, color);
    }

    class ImageLoader{
    public:
        virtual void Load(Image & image, std::string filename) = 0;
        virtual void Save(Image & image, std::string filename = "") = 0;
    };

    Square Image::GetImageBounds() {
        return Square(0, 0, GetWidth(), GetHeight());
    }
}


#endif //UNTITLED_IMAGE_H
