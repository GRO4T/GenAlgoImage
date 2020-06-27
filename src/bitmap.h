//
// Created by DamianPC on 6/26/2020.
//

#ifndef UNTITLED_BITMAP_H
#define UNTITLED_BITMAP_H

#include "figures.h"
#include "image.h"

const unsigned int BMP_HEADER_LENGTH = 14;

class Bitmap : public Image{
public:
    unsigned int size;
    unsigned int pixelDataOffset;
    char * data;

    ~Bitmap(){
        if (data != nullptr){
            delete data;
        }
    }

    unsigned int getWidth() const;
    void setWidth(unsigned int width);
    unsigned int getHeight() const;
    void setHeight(unsigned int height);

    Color getPixelColor(int x, int y) override;

    void clearColor(Color color) override;
    void drawPixel(int x, int y, Color color) override;
    void drawCircle(unsigned int posx, unsigned int posy, double radius, Color color);
private:
    unsigned int width;
    unsigned int paddedWidth;
    unsigned int height;
};

void Bitmap::drawPixel(int x, int y, Color color) {
    if (x >= width | x < 0 | y >= height | y < 0 | width < 0 | height < 0)
        return;
    int byteNumber = y * paddedWidth * 3 + 3 * x + pixelDataOffset;
    data[byteNumber] = color.red;
    data[byteNumber + 1] = color.green;
    data[byteNumber + 2] = color.blue;
}

Color Bitmap::getPixelColor(int x, int y) {
    return *new Color();
}

unsigned int Bitmap::getWidth() const {
    return width;
}

void Bitmap::setWidth(unsigned int width) {
    Bitmap::width = width;
    paddedWidth = width + ((4 - width % 4) % 4);
}

unsigned int Bitmap::getHeight() const {
    return height;
}

void Bitmap::setHeight(unsigned int height) {
    Bitmap::height = height;
}

void Bitmap::clearColor(Color color) {
    Image::drawSquare(0, 0, width, height, color);
}

void Bitmap::drawCircle(unsigned int posx, unsigned int posy, double radius, Color color) {
    if (radius < 0.0f)
        return;
    for (int y = posy - radius; y < posy + radius; y++){
        for (int x = posx - radius; x < posx + radius; x++){
            if ((x >= 0 && x < width) && (y >= 0 && y < height)){
                int dx = x - posx;
                int dy = y - posy;
                if (sqrt((double)(dx * dx + dy * dy)) < radius)
                    drawPixel(x, y, color);
            }
        }
    }
}

#endif //UNTITLED_BITMAP_H
