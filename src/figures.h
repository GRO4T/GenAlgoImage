//
// Created by DamianPC on 6/26/2020.
//

#ifndef UNTITLED_FIGURES_H
#define UNTITLED_FIGURES_H

#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <algorithm>

class Color{
public:
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    Color(uint8_t red, uint8_t green, uint8_t blue) : red(red), green(green), blue(blue) {}
    Color() { red = rand() % 256; green = rand() % 256; blue = rand() % 256; }

    bool isNoColor() const {
        return noColor;
    }

    void setNoColor(bool noColor) {
        Color::noColor = noColor;
    }

    // overloaded operators
    friend std::ostream& operator<<(std::ostream& os, const Color &color);
    Color operator+(const Color& color);
    Color operator-(const Color& color);
private:
    bool noColor = false;
};

std::ostream &operator<<(std::ostream &os, const Color &color) {
    os << "(R, G, B) = (" << (unsigned int)color.red << ", " <<
       (unsigned int)color.green << ", " <<
       (unsigned int)color.blue << ")";

    return os;
}

Color Color::operator+(const Color &color) {
    int red = std::max(0, std::min((int)(this->red + color.red), 255));
    int green = std::max(0, std::min((int)(this->green + color.green), 255));
    int blue = std::max(0, std::min((int)(this->blue + color.blue), 255));
    return Color(red, blue, green);
}

Color Color::operator-(const Color &color) {
    int red = std::max(0, std::min((int)(this->red - color.red), 255));
    int green = std::max(0, std::min((int)(this->green - color.green), 255));
    int blue = std::max(0, std::min((int)(this->blue - color.blue), 255));
    return Color(red, blue, green);
}

class Square{
public:
    int x;
    int y;
    int width;
    int height;

    Square(int posx, int posy, int width, int height) : x(posx), y(posy), width(width), height(height) {}
    Square() {}
    void randomize(Square bounds);
};

void Square::randomize(Square bounds) {
    this->x = rand() % bounds.width + bounds.x;
    this->y = rand() % bounds.height + bounds.y;
    this->width = rand() % (bounds.width - this->x);
    this->height = rand() % (bounds.height - this->y);
}

class Circle{
public:
    int x;
    int y;
    double radius;

    Circle(int x, int y, double radius) : x(x), y(y), radius(radius) {}
    Circle() {}
};

#endif //UNTITLED_FIGURES_H
