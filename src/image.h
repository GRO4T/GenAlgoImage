//
// Created by DamianPC on 6/27/2020.
//

#ifndef UNTITLED_IMAGE_H
#define UNTITLED_IMAGE_H

#include "figures.h"

class Image{
public:
    virtual void drawPixel(int x, int y, Color color) = 0;
    virtual Color getPixelColor(int x, int y) = 0;

    virtual unsigned int getWidth() const = 0;
    virtual void setWidth(unsigned int width) = 0;
    virtual unsigned int getHeight() const = 0;
    virtual void setHeight(unsigned int height) = 0;

    virtual void clearColor(Color color) = 0;
    virtual void drawSquare(int posx, int posy, unsigned int width, unsigned int height, Color color);
    virtual void drawSquare(Square square, Color color);
    //virtual void drawCircle(int x, int y, double radius, Color color);
    //virtual void drawCircle(Circle circle, Color color);
};

void Image::drawSquare(int posx, int posy, unsigned int width, unsigned int height, Color color) {
    for (int y = posy; y < posy + height; y++){
        for (int x = posx; x < posx + width; x++){
            drawPixel(x, y, color);
        }
    }
}

void Image::drawSquare(Square square, Color color) {
    drawSquare(square.posx, square.posy, square.width, square.height, color);
}



#endif //UNTITLED_IMAGE_H
