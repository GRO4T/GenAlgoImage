//
// Created by DamianPC on 6/27/2020.
//

#ifndef UNTITLED_IMAGE_H
#define UNTITLED_IMAGE_H

#include <SFML/Graphics/Image.hpp>
#include "figures.h"

class Image{
public:
    virtual void create(unsigned int width, unsigned int height, Color imageColor) = 0;

    virtual bool isPointInBounds(int x, int y) { return (x >= 0 && x < getWidth()) && (y >= 0 && y < getHeight()); }

    virtual void drawPixel(int x, int y, Color color) = 0;
    virtual Color getPixelColor(int x, int y) = 0;

    virtual unsigned int getWidth() const = 0;
    virtual unsigned int getHeight() const = 0;

    virtual void clearColor(Color color);
    virtual void drawSquare(int posx, int posy, unsigned int width, unsigned int height, Color color);
    virtual void drawSquare(Square square, Color color);
    virtual void drawCircle(int posx, int posy, double radius, Color color);
    virtual void drawCircle(Circle circle, Color color);

    virtual void loadToSFImage(sf::Image& img) = 0;
};

void Image::clearColor(Color color) {
    drawSquare(0, 0, getWidth(), getHeight(), color);
}

void Image::drawSquare(int posx, int posy, unsigned int width, unsigned int height, Color color) {
    for (int y = posy; y < posy + height; y++){
        for (int x = posx; x < posx + width; x++){
            drawPixel(x, y, color);
        }
    }
}

void Image::drawSquare(Square square, Color color) {
    drawSquare(square.x, square.y, square.width, square.height, color);
}

void Image::drawCircle(int posx, int posy, double radius, Color color) {
    if (radius < 0.0f)
        return;
    for (int y = posy - radius; y < posy + radius; y++){
        for (int x = posx - radius; x < posx + radius; x++){
            if (isPointInBounds(x, y)){
                int dx = x - posx;
                int dy = y - posy;
                if (sqrt((double)(dx * dx + dy * dy)) < radius)
                    drawPixel(x, y, color);
            }
        }
    }
}

void Image::drawCircle(Circle circle, Color color) {
    drawCircle(circle.x, circle.y, circle.radius, color);
}

class ImageLoader{
public:
    virtual void load(Image & image, std::string filename) = 0;
    virtual void save(Image & image, std::string filename = "") = 0;
};


#endif //UNTITLED_IMAGE_H
