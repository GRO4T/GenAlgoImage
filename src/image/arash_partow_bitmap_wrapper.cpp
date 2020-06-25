//
// Created by damian on 7/31/20.
//

#include "arash_partow_bitmap_wrapper.h"

namespace gen_algo_image {
    ArashPartowBitmapWrapper::ArashPartowBitmapWrapper() : drawer(image) {
        drawer.pen_width(-1);
        Load("../res/lena.bmp");
    }

    ArashPartowBitmapWrapper::ArashPartowBitmapWrapper(const ArashPartowBitmapWrapper &b2)
            : drawer(image) {
        this->image = b2.image;
    }

    ArashPartowBitmapWrapper &ArashPartowBitmapWrapper::operator=(const ArashPartowBitmapWrapper &b2) {
        this->image = b2.image;
        return *this;
    }

    void ArashPartowBitmapWrapper::Create(unsigned int width, unsigned int height, Color imageColor) {
        image.set_region(0, 0, width, height, imageColor.red, imageColor.green, imageColor.blue);
    }

    void ArashPartowBitmapWrapper::Load(const std::string &filename) {
        image = bitmap_image(filename);
    }

    void ArashPartowBitmapWrapper::Save(const std::string &filename) {
        image.save_image(filename);
    }

    bool ArashPartowBitmapWrapper::IsPointInBounds(int x, int y) {
        return Image::IsPointInBounds(x, y);
    }

    void ArashPartowBitmapWrapper::DrawPixel(int x, int y, Color color) {
        image.set_pixel(x, y, color.red, color.green, color.blue);
    }

    Color ArashPartowBitmapWrapper::GetPixelColor(int x, int y) {
        Color color = Color();
        image.get_pixel(x, y, color.red, color.green, color.blue);
        return color;
    }

    void ArashPartowBitmapWrapper::GetPixelColor(const int &x, const int &y, Color &color) {
        image.get_pixel(x, y, color.red, color.green, color.blue);
    }

    unsigned int ArashPartowBitmapWrapper::GetWidth() const {
        return image.width();
    }

    unsigned int ArashPartowBitmapWrapper::GetHeight() const {
        return image.height();
    }

    void ArashPartowBitmapWrapper::ClearColor(Color color) {
        image.set_region(0, 0, image.width(), image.height(), color.red, color.green, color.blue);
    }

    void
    ArashPartowBitmapWrapper::DrawSquare(int posx, int posy, unsigned int width, unsigned int height, Color color) {
        image.set_region(posx, posy, width, height, color.red, color.green, color.blue);
    }

    void ArashPartowBitmapWrapper::DrawSquare(Square square, Color color) {
        ArashPartowBitmapWrapper::DrawSquare(square.x, square.y, square.width, square.height, color);
    }

    void ArashPartowBitmapWrapper::DrawCircle(const int &posx, const int &posy, int radius, const Color &color) {
        /*
        drawer.pen_color(color.red, color.green, color.blue);
        while (radius >= 0){
            drawer.circle(posx, posy, radius--);
        }*/
        Image::DrawCircle(posx, posy, radius, color);
    }

    void ArashPartowBitmapWrapper::DrawCircle(const Circle &circle, const Color &color) {
        ArashPartowBitmapWrapper::DrawCircle(circle.x, circle.y, circle.radius, color);
    }

    void ArashPartowBitmapWrapper::LoadToSFImage(sf::Image &img) {
        image.save_image("../res/temp.bmp");
        img.loadFromFile("../res/temp.bmp");
    }

    Square ArashPartowBitmapWrapper::GetImageBounds() {
        return Image::GetImageBounds();
    }
}

