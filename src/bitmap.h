//
// Created by DamianPC on 6/26/2020.
//

#ifndef UNTITLED_BITMAP_H
#define UNTITLED_BITMAP_H

#include <string>
#include <fstream>

#include "figures.h"
#include "image.h"

/* Classes in this file (search using <classname> pattern, e.g. <Bitmap>)
 *      Bitmap
 *      BitmapLoader
 */

const unsigned int BMP_HEADER_LENGTH = 14;

/* <Bitmap> */

class Bitmap : public Image{
    friend class BitmapLoader;
public:
    unsigned int size;
    unsigned int pixelDataOffset;
    char * data;

    ~Bitmap(){
        if (data != nullptr){
            delete data;
        }
    }

    void create(unsigned int width, unsigned int height, Color imageColor) override {
        throw std::runtime_error("bitmap: create not implemented!");
    }

    unsigned int getWidth() const override ;
    unsigned int getHeight() const override ;
    void drawPixel(int x, int y, Color color) override ;
    Color getPixelColor(int x, int y) override ;

    void loadToSFImage(sf::Image &img) override;

private:
    unsigned int width;
    unsigned int paddedWidth;
    unsigned int height;

    void setWidth(unsigned int width);
    void setHeight(unsigned int height);
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
    Color color;
    if (x >= width | x < 0 | y >= height | y < 0 | width < 0 | height < 0){
        color.setNoColor(true);
    }
    else{
        int byteNumber = y * paddedWidth * 3 + 3 * x + pixelDataOffset;
        color.red = data[byteNumber];
        color.green = data[byteNumber + 1];
        color.blue = data[byteNumber + 2];
    }
    return color;
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

void Bitmap::loadToSFImage(sf::Image &img) {
    img.loadFromMemory(data, size);
}

/* <BitmapLoader> */

class BitmapLoader : public ImageLoader{
public:
    ~BitmapLoader(){
        if (buffer != nullptr) delete buffer;
    }

    void load(Image & image, std::string filename) override ;
    void save(Image & image, std::string filename = "") override ;


    static void readBytes(std::string filename, char * buffer, unsigned int n, unsigned int startpos = 0);
    template<typename T>
    static T hexConvert(std::string bytes);
private:
    std::string filename;
    unsigned int dibHeaderLength;

    char * buffer;

    void processHeader(Bitmap & bitmap);
    void processDIBHeader(Bitmap & bitmap);
    void processBody(Bitmap & bitmap);

    // DIB headers
    void parseBITMAPINFOHEADER(Bitmap & bitmap);
};


void BitmapLoader::load(Image& image, std::string filename) {
    this->filename = filename;
    Bitmap& bitmap = (Bitmap&)image;
    try{
        processHeader(bitmap);
        processDIBHeader(bitmap);
        processBody(bitmap);
    }
    catch(std::runtime_error e){
        std::cerr << e.what() << std::endl;
        return;
    }
}

void BitmapLoader::save(Image& image, std::string filename) {
    if (filename == "") filename = this->filename;

    Bitmap& bitmap = (Bitmap&)image;
    std::ofstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try{
        file.open(filename, std::ios::out | std::ios::binary);
        file.write(bitmap.data, bitmap.size);
    }
    catch (std::ofstream::failure e) {
        std::cerr << "Exception opening file: " << filename << " \n" << "What:\t" << e.what();
        std::exit(EXIT_FAILURE);
    }
}

void BitmapLoader::processHeader(Bitmap & bitmap) {
    char header[BMP_HEADER_LENGTH];
    readBytes(this->filename, header, BMP_HEADER_LENGTH);
    std::string stringBuffer(&header[0], &header[BMP_HEADER_LENGTH]);
    bitmap.size = hexConvert<unsigned int>(stringBuffer.substr(2, 4));

    this->buffer = new char[bitmap.size];
}

void BitmapLoader::processDIBHeader(Bitmap & bitmap) {
    readBytes(this->filename, this->buffer, bitmap.size, 0);
    std::string buf(&(this->buffer[BMP_HEADER_LENGTH]), &(this->buffer[BMP_HEADER_LENGTH + 4]));
    dibHeaderLength = hexConvert<unsigned int>(buf);
    switch(dibHeaderLength){
        case 40: parseBITMAPINFOHEADER(bitmap); break;
        default: throw std::runtime_error("DIB Header not supported!");
    }
}

void BitmapLoader::processBody(Bitmap &bitmap) {
    bitmap.data = new char[bitmap.size];
    memcpy(bitmap.data, this->buffer, bitmap.size);
}

void BitmapLoader::readBytes(std::string filename, char * buffer, unsigned int n, unsigned int startpos) {
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try{
        file.open(filename, std::ios::binary);
        file.seekg(startpos);
        file.read(buffer, n);
    }
    catch (std::ifstream::failure e) {
        std::cerr << "Exception opening file: " << filename << " \n" << "What:\t" << e.what();
        std::exit(EXIT_FAILURE);
    }
}

template<typename T>
T BitmapLoader::hexConvert(std::string bytes){
    T accumulator = 0;
    for (int i = 0; i < bytes.length(); i++){
        accumulator += std::pow(16, 2*i) * (std::uint8_t)bytes[i];
    }
    return accumulator;
}

void BitmapLoader::parseBITMAPINFOHEADER(Bitmap &bitmap) {
    bitmap.pixelDataOffset = 54;
    std::string stringBuffer1(&(this->buffer[BMP_HEADER_LENGTH + 4]),&(this->buffer[BMP_HEADER_LENGTH + 8]));
    bitmap.setWidth(hexConvert<unsigned int>(stringBuffer1));
    std::string stringBuffer2(&(this->buffer[BMP_HEADER_LENGTH + 8]),&(this->buffer[BMP_HEADER_LENGTH + 12]));
    bitmap.setHeight(hexConvert<unsigned int>(stringBuffer2));
}

#endif //UNTITLED_BITMAP_H
