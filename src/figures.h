//
// Created by DamianPC on 6/26/2020.
//

#ifndef UNTITLED_FIGURES_H
#define UNTITLED_FIGURES_H

#include <cstdlib>
#include <cstdint>

class Color{
public:
    Color(uint8_t red, uint8_t green, uint8_t blue) : red(red), green(green), blue(blue) {}
    Color() { red = rand() % 256; green = rand() % 256; blue = rand() % 256; }
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

class Square{
public:
    int posx;
    int posy;
    int width;
    int height;

    Square(int posx, int posy, int width, int height) : posx(posx), posy(posy), width(width), height(height) {}
    Square() {}
    void randomize(Square bounds);
};

class Circle{

};

void Square::randomize(Square bounds) {
    this->posx = rand() % bounds.width + bounds.posx;
    this->posy = rand() % bounds.height + bounds.posy;
    this->width = rand() % (bounds.width - this->posx);
    this->height = rand() % (bounds.height - this->posy);
}

#endif //UNTITLED_FIGURES_H
