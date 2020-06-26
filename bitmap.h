//
// Created by DamianPC on 6/26/2020.
//

#ifndef UNTITLED_BITMAP_H
#define UNTITLED_BITMAP_H

const unsigned int BMP_HEADER_LENGTH = 14;

class Color{
public:
    Color(uint8_t red, uint8_t green, uint8_t blue) : red(red), green(green), blue(blue) {}
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

class Bitmap{
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

    void clearColor(Color color);
    void drawPixel(int x, int y, Color color);
    void drawSquare(int posx, int posy, int width, int height, Color color);
    //void drawCircle(int posx, int posy, int radius, Color color);
private:
    unsigned int width;
    unsigned int paddedWidth;
    unsigned int height;
};

void Bitmap::drawPixel(int x, int y, Color color) {
    int byteNumber = y * paddedWidth * 3 + 3 * x + pixelDataOffset;
    data[byteNumber] = color.red;
    data[byteNumber + 1] = color.green;
    data[byteNumber + 2] = color.blue;
}

void Bitmap::drawSquare(int posx, int posy, int width, int height, Color color) {
    for (int y = posy; y < posy + height; y++){
        for (int x = posx; x < posx + width; x++){
            drawPixel(x, y, color);
        }
    }
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
    drawSquare(0, 0, width, height, color);
}

#endif //UNTITLED_BITMAP_H
