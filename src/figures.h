//
// Created by DamianPC on 6/26/2020.
//

#ifndef UNTITLED_FIGURES_H
#define UNTITLED_FIGURES_H

#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <algorithm>

#define MIN_CIRCLE_RADIUS 10
#define MAX_CIRCLE_RADIUS 70

namespace gen_algo_image{
    class Color{
    public:
        uint8_t red;
        uint8_t green;
        uint8_t blue;

        Color(uint8_t red, uint8_t green, uint8_t blue) : red(red), green(green), blue(blue) {}
        Color() { red = rand() % 256; green = rand() % 256; blue = rand() % 256; }

        Color Diff(Color color);

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
        return Color(red, green, blue);
    }

    Color Color::operator-(const Color &color) {
        int red = std::max(0, std::min((int)(this->red - color.red), 255));
        int green = std::max(0, std::min((int)(this->green - color.green), 255));
        int blue = std::max(0, std::min((int)(this->blue - color.blue), 255));
        return Color(red, green, blue);
    }

    Color Color::Diff(Color color) {
        int red = std::abs((int)(this->red - color.red));
        int green = std::abs((int)(this->green - color.green));
        int blue = std::abs((int)(this->blue - color.blue));
        return Color(red, green, blue);
    }

    class Square{
    public:
        int x;
        int y;
        int width;
        int height;

        Square(int posx, int posy, int width, int height) : x(posx), y(posy), width(width), height(height) {}
        Square() {}
        void Randomize(Square bounds);
    };

    void Square::Randomize(Square bounds) {
        this->x = rand() % bounds.width + bounds.x;
        this->y = rand() % bounds.height + bounds.y;
        this->width = rand() % (bounds.width - this->x);
        this->height = rand() % (bounds.height - this->y);
    }

    class Circle{
    public:
        int x;
        int y;
        int radius;

        Circle(int x, int y, int radius) : x(x), y(y), radius(radius) {}
        Circle() {}
        void Randomize(Square bounds);
    };

    void Circle::Randomize(Square bounds) {
        this->x = rand() % bounds.width + bounds.x;
        this->y = rand() % bounds.height + bounds.y;
        this->radius = rand() % MAX_CIRCLE_RADIUS + MIN_CIRCLE_RADIUS;
    }

    class ColoredSquare{
    public:
        ColoredSquare(const Square &square, const Color &color) : square(square), color(color) {}
        ColoredSquare(Square bounds) { square.Randomize(bounds); color = Color(); }
        Square square;
        Color color;
    };

    class ColoredCircle{
    public:
        ColoredCircle(const Circle &circle, const Color &color) : circle(circle), color(color) {}
        ColoredCircle(Square bounds) { circle.Randomize(bounds); color = Color(); }
        Circle circle;
        Color color;
    };
}

#endif //UNTITLED_FIGURES_H
